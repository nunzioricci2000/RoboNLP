from os import getenv

openai_api_key = getenv("OPENAI_API_KEY")
if not openai_api_key:
    raise ValueError("OPENAI_API_KEY not set")

openai_base_url = getenv("OPENAI_BASE_URL", "https://generativelanguage.googleapis.com/v1beta/openai/")

openai_model = getenv("OPENAI_MODEL", "gemini-2.0-flash")

server_ip = getenv("SERVER_IP", "server")

server_port = getenv("SERVER_PORT", "1025")

furhat_host = getenv("FURHAT_HOST", "host.docker.internal")

furhat_voice = getenv("FURHAT_VOICE", "Bianca")