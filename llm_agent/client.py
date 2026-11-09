"""Multi-provider LLM Client supporting Gemini, OpenAI, Anthropic, Ollama, and Mock testing.

Uses standard HTTP requests (urllib.request / requests) so it works out-of-the-box
without requiring external SDK dependencies, while maintaining robust error handling.
"""

import json
import os
import re
import time
import urllib.request
import urllib.error
from abc import ABC, abstractmethod
from pathlib import Path
from typing import List, Dict, Any, Optional


def load_env_file():
    """Load key-value pairs from .env in project root, prioritizing valid .env keys over placeholders."""
    repo_root = Path(__file__).resolve().parent.parent
    env_file = repo_root / ".env"
    if env_file.exists():
        with open(env_file, "r") as f:
            for line in f:
                line = line.strip()
                if line and not line.startswith("#") and "=" in line:
                    k, v = line.split("=", 1)
                    k = k.strip()
                    v = v.strip().strip("'").strip('"')
                    curr = os.environ.get(k, "").strip()
                    # Override if unset or if existing environ is placeholder text
                    if not curr or "your-" in curr or "placeholder" in curr or (v.startswith("sk-") and not curr.startswith("sk-")):
                        os.environ[k] = v
                    elif v.startswith("sk-") and curr.startswith("sk-") and v != curr:
                        # If .env has a real sk- key, prefer the .env key
                        os.environ[k] = v


load_env_file()


class LLMClient(ABC):
    """Abstract base class for LLM API providers."""

    @abstractmethod
    def generate(self, messages: List[Dict[str, str]], system_prompt: Optional[str] = None, temperature: float = 0.4) -> str:
        """Send chat messages and return assistant response text."""
        pass


class GeminiClient(LLMClient):
    """Google Gemini API client using standard HTTP requests."""

    def __init__(self, api_key: Optional[str] = None, model: str = "gemini-2.5-flash"):
        self.api_key = api_key or os.environ.get("GEMINI_API_KEY", "")
        self.model = model
        if not self.api_key:
            raise ValueError("GEMINI_API_KEY environment variable or argument is required for GeminiClient.")

    def generate(self, messages: List[Dict[str, str]], system_prompt: Optional[str] = None, temperature: float = 0.4) -> str:
        url = f"https://generativelanguage.googleapis.com/v1beta/models/{self.model}:generateContent?key={self.api_key}"

        # Format Gemini contents
        contents = []
        for msg in messages:
            role = "user" if msg["role"] == "user" else "model"
            contents.append({"role": role, "parts": [{"text": msg["content"]}]})

        payload: Dict[str, Any] = {
            "contents": contents,
            "generationConfig": {"temperature": temperature, "maxOutputTokens": 2048},
        }

        if system_prompt:
            payload["systemInstruction"] = {"parts": [{"text": system_prompt}]}

        req = urllib.request.Request(
            url,
            data=json.dumps(payload).encode("utf-8"),
            headers={"Content-Type": "application/json"},
            method="POST",
        )

        try:
            with urllib.request.urlopen(req, timeout=60) as resp:
                data = json.loads(resp.read().decode("utf-8"))
                candidates = data.get("candidates", [])
                if candidates:
                    parts = candidates[0].get("content", {}).get("parts", [])
                    if parts:
                        return parts[0].get("text", "")
                return ""
        except urllib.error.HTTPError as e:
            err_body = e.read().decode("utf-8")
            raise RuntimeError(f"Gemini API HTTP Error {e.code}: {err_body}")
        except Exception as e:
            raise RuntimeError(f"Gemini API Request Error: {e}")


class OpenAIClient(LLMClient):
    """OpenAI API client using standard HTTP requests."""

    def __init__(self, api_key: Optional[str] = None, model: str = "gpt-4o-mini", base_url: str = "https://api.openai.com/v1"):
        self.api_key = api_key or os.environ.get("OPENAI_API_KEY", "")
        self.model = model
        self.base_url = base_url.rstrip("/")
        if not self.api_key and "localhost" not in self.base_url:
            raise ValueError("OPENAI_API_KEY environment variable or argument is required for OpenAIClient.")

    def generate(self, messages: List[Dict[str, str]], system_prompt: Optional[str] = None, temperature: float = 0.4) -> str:
        url = f"{self.base_url}/chat/completions"

        full_msgs = []
        if system_prompt:
            full_msgs.append({"role": "system", "content": system_prompt})
        full_msgs.extend(messages)

        payload = {
            "model": self.model,
            "messages": full_msgs,
            "temperature": temperature,
            "max_tokens": 2048,
        }

        headers = {
            "Content-Type": "application/json",
            "Authorization": f"Bearer {self.api_key}",
        }

        req = urllib.request.Request(
            url,
            data=json.dumps(payload).encode("utf-8"),
            headers=headers,
            method="POST",
        )

        try:
            with urllib.request.urlopen(req, timeout=60) as resp:
                data = json.loads(resp.read().decode("utf-8"))
                choices = data.get("choices", [])
                if choices:
                    return choices[0].get("message", {}).get("content", "")
                return ""
        except urllib.error.HTTPError as e:
            err_body = e.read().decode("utf-8")
            raise RuntimeError(f"OpenAI API HTTP Error {e.code}: {err_body}")
        except Exception as e:
            raise RuntimeError(f"OpenAI API Request Error: {e}")


class AnthropicClient(LLMClient):
    """Anthropic Claude API client using standard HTTP requests."""

    def __init__(self, api_key: Optional[str] = None, model: str = "claude-haiku-5-5"):
        self.api_key = api_key or os.environ.get("ANTHROPIC_API_KEY", "")
        # Normalize model aliases for Anthropic
        m_lower = model.lower().replace(".", "-")
        if "5-5" in m_lower or "5.5" in model:
            if "sonnet" in m_lower:
                self.model = "claude-sonnet-5-5"
            elif "opus" in m_lower:
                self.model = "claude-opus-5-5"
            else:
                self.model = "claude-haiku-5-5"
        elif "4-5" in m_lower or "4.5" in model:
            if "opus" in m_lower:
                self.model = "claude-opus-4-5-20251101"
            else:
                self.model = "claude-haiku-4-5-20251001"
        elif "haiku" in m_lower:
            self.model = "claude-haiku-5-5"
        elif "sonnet" in m_lower:
            self.model = "claude-sonnet-5-5"
        else:
            self.model = model

        if not self.api_key:
            raise ValueError(
                "ANTHROPIC_API_KEY is not set. Please set it via `export ANTHROPIC_API_KEY='sk-ant-...'` "
                "or create a .env file containing ANTHROPIC_API_KEY=sk-ant-..."
            )
        masked_key = "..." + self.api_key[-4:] if len(self.api_key) > 8 else "<set>"
        print(f"[*] Anthropic Client initialized (Model: {self.model}, Key: {masked_key})")

    def generate(self, messages: List[Dict[str, str]], system_prompt: Optional[str] = None, temperature: float = 0.4) -> str:
        url = "https://api.anthropic.com/v1/messages"

        payload: Dict[str, Any] = {
            "model": self.model,
            "messages": messages,
            # Claude 5.5 models think adaptively by default and thinking tokens count
            # against max_tokens; 4096 truncated long replies (some to empty text).
            "max_tokens": 16000,
        }
        # Newer Claude 5.x / 4.x models deprecate explicit temperature
        if "-5" not in self.model and "-4" not in self.model and temperature is not None:
            payload["temperature"] = temperature

        if system_prompt:
            payload["system"] = system_prompt

        headers = {
            "Content-Type": "application/json",
            "x-api-key": self.api_key,
            "anthropic-version": "2023-06-01",
        }

        last_err = None
        for attempt in range(4):
            req = urllib.request.Request(
                url,
                data=json.dumps(payload).encode("utf-8"),
                headers=headers,
                method="POST",
            )
            try:
                with urllib.request.urlopen(req, timeout=600) as resp:
                    data = json.loads(resp.read().decode("utf-8"))
                usage = data.get("usage", {})
                self.last_stop_reason = data.get("stop_reason")
                self.last_usage = usage
                print(f"    [Anthropic API] Model: {data.get('model', self.model)} | Tokens: "
                      f"{usage.get('input_tokens', 0)} in, {usage.get('output_tokens', 0)} out | "
                      f"stop_reason: {self.last_stop_reason}")
                text_parts = [b.get("text", "") for b in data.get("content", []) if b.get("type") == "text"]
                return "\n".join(text_parts).strip()
            except urllib.error.HTTPError as e:
                err_body = e.read().decode("utf-8")
                last_err = RuntimeError(f"Anthropic API HTTP Error {e.code}: {err_body}")
                if e.code not in (429, 500, 502, 503, 529):
                    raise last_err
            except Exception as e:
                last_err = RuntimeError(f"Anthropic API Request Error: {e}")
            wait = 10 * (attempt + 1)
            print(f"    [Anthropic API] transient error ({last_err}); retrying in {wait}s")
            time.sleep(wait)
        raise last_err


class MockClient(LLMClient):
    """Deterministic / simulated LLM client for offline automated testing and validation."""

    def __init__(self, benchmark_name: str = "dot_product", slot_count: int = 4):
        self.benchmark_name = benchmark_name
        self.slot_count = slot_count
        self.step = 0

    def generate(self, messages: List[Dict[str, str]], system_prompt: Optional[str] = None, temperature: float = 0.4) -> str:
        self.step += 1

        # Check last message for errors or feedback
        last_msg = messages[-1]["content"] if messages else ""

        if "dot_product" in self.benchmark_name:
            if self.step == 1:
                return (
                    "I will pack vector v1 and vector v2 into parallel SIMD ciphertexts and evaluate the elementwise product with VecMul, "
                    "followed by a logarithmic rotation tree reduction.\n\n"
                    "PROPOSED_EXPRESSION:\n"
                    "(VecAddRot (VecAddRot (VecMul (Vec v1_0 v1_2 v1_1 v1_3) (Vec v2_0 v2_2 v2_1 v2_3)) 2) 1)"
                )
            elif self.step == 2:
                return (
                    "Analyzing the compiler feedback: the previous proposal achieved 2.5x speedup with multiplicative depth 1. "
                    "I will now test an alternative rotation alignment order (shift 1 first, then shift 2) to see if noise accumulation or latency improves.\n\n"
                    "PROPOSED_EXPRESSION:\n"
                    "(VecAddRot (VecAddRot (VecMul (Vec v1_0 v1_1 v1_2 v1_3) (Vec v2_0 v2_1 v2_2 v2_3)) 1) 2)"
                )
            else:
                return (
                    "Refining the best schedule with optimal SIMD slot alignment.\n\n"
                    "PROPOSED_EXPRESSION:\n"
                    "(VecAddRot (VecAddRot (VecMul (Vec v1_0 v1_2 v1_1 v1_3) (Vec v2_0 v2_2 v2_1 v2_3)) 2) 1)"
                )

        elif "nn_linear" in self.benchmark_name:
            if self.slot_count == 4:
                return (
                    "For nn_linear with 4 slots, we pack weight rows and inputs to compute parallel matrix-vector product with bias accumulation.\n\n"
                    "PROPOSED_EXPRESSION:\n"
                    "(VecAdd (VecAddRot (VecAddRot (VecMul (Vec w_0_0 w_1_0 w_2_0 w_3_0) (Vec x_0 x_0 x_0 x_0)) 2) 1) (Vec b_0 b_1 b_2 b_3))"
                )
            else:
                return (
                    "Vectorizing nn_linear layer with parallel slot broadcast.\n\n"
                    "PROPOSED_EXPRESSION:\n"
                    "(VecAddRot (VecMul (Vec x_0 x_1) (Vec w_0_0 w_0_1)) 1)"
                )

        elif "nn_avgpool2d" in self.benchmark_name:
            return (
                "For average pooling 2x2, no multiplications are required (depth 0). We pack adjacent pooling window elements and accumulate.\n\n"
                "PROPOSED_EXPRESSION:\n"
                "(VecAddRot (VecAdd (Vec x_1_1 x_1_3 x_3_1 x_3_3 x_0_0 x_0_2 x_2_0 x_2_2) (Vec x_1_0 x_1_2 x_3_0 x_3_2 x_0_1 x_0_3 x_2_1 x_2_3)) 4)"
            )

        elif "quad_ckks" in self.benchmark_name:
            return (
                "For Quad activation y = x^2, we optimize the ciphertext square schedule.\n\n"
                "PROPOSED_EXPRESSION:\n"
                "(* x x)"
            )

        elif "batchnorm" in self.benchmark_name:
            return (
                "For BatchNorm inference, we fold running mean and variance into affine parameters A and B.\n\n"
                "PROPOSED_EXPRESSION:\n"
                "(+ (* x A) B)"
            )

        elif "chebyshev" in self.benchmark_name:
            if self.step == 1:
                return (
                    "Evaluating Chebyshev polynomial using balanced binary exponentiation power tree to minimize multiplicative depth.\n\n"
                    "PROPOSED_EXPRESSION:\n"
                    "(+ (+ (* c5 (* (* x x) (* (* x x) x))) (* c3 (* (* x x) x))) (+ (* c1 x) c0))"
                )
            else:
                return (
                    "Paterson-Stockmeyer decomposition: factorizing polynomial into quotient and remainder to eliminate 2 rescale levels.\n\n"
                    "PROPOSED_EXPRESSION:\n"
                    "(+ (* (* x x) (+ (* c5 (* x x)) c3)) (+ (* c1 x) c0))"
                )

        elif "relu_ckks" in self.benchmark_name:
            return (
                "Minimax composite sign approximation for ReLU evaluation.\n\n"
                "PROPOSED_EXPRESSION:\n"
                "(* x (* x x))"
            )

        # Generic fallback candidate
        return (
            "Proposing optimized candidate.\n\n"
            "PROPOSED_EXPRESSION:\n"
            "(+ x x)"
        )


def create_llm_client(
    provider: str = "mock",
    model: Optional[str] = None,
    api_key: Optional[str] = None,
    benchmark_name: str = "dot_product",
    slot_count: int = 4,
) -> LLMClient:
    """Factory to create the appropriate LLMClient."""
    prov = provider.lower().strip()
    if prov == "gemini":
        model_name = model or "gemini-2.5-flash"
        return GeminiClient(api_key=api_key, model=model_name)
    elif prov in ("openai", "gpt"):
        model_name = model or "gpt-4o-mini"
        return OpenAIClient(api_key=api_key, model=model_name)
    elif prov in ("anthropic", "claude"):
        model_name = model or "claude-haiku-5-5"
        return AnthropicClient(api_key=api_key, model=model_name)
    elif prov == "mock":
        return MockClient(benchmark_name=benchmark_name, slot_count=slot_count)
    else:
        raise ValueError(f"Unknown LLM provider '{provider}'. Supported providers: 'mock', 'gemini', 'openai', 'anthropic'.")
