from openai import OpenAI
import dotenv


dotenv.load_dotenv()
#cognitivecomputations/dolphin3.0-mistral-24b:free
# MODEL = "deepseek/deepseek-r1-0528-qwen3-8b:free"
MODEL = "google/gemma-3n-e4b-it:free"
MODEL = "meta-llama/llama-4-scout-17b-16e-instruct"

client = OpenAI(
#   base_url="https://openrouter.ai/api/v1",
    base_url="https://api.groq.com/openai/v1",
#   api_key=dotenv.get_key('.env',"OPENROUTER_API_KEY"),
  api_key=dotenv.get_key('.env',"GROQ_API_KEY"),
)

def get_response(prompt):
    completion = client.chat.completions.create(
    extra_headers={
        # "HTTP-Referer": "<YOUR_SITE_URL>", # Optional. Site URL for rankings on openrouter.ai.
        # "X-Title": "<YOUR_SITE_NAME>", # Optional. Site title for rankings on openrouter.ai.
    },
    extra_body={},
    model= MODEL,
    messages=[
        {
            "role": "user",
            "content": f"User request:{prompt}"
        },
        {
            "role": "system",
            "content" : 'Translate the user request to a command string in the format "component value". Rules: led<1-7>:<0|1>; lcd <text>; traffic:<1=red,2=yellow,3=green>; servo <angle>; buzzer <1-1000 freq>. Chain multiple commands with a comma. Examples: "turn led 1 on and write hello" -> "led1:1,lcd hello", "buzz low" -> "buzzer 10" . There is a space between buzzer and 10. Output ONLY the command string. If the statement is is not a direct command and does not mention keywords relating to any of the components, return an empty string instead. Never return something that is not a command or and nothing. Your exact response is used to  make requests so do not include any additional text or explanation. If the user request is not a command, return an empty string. Provide your output in a json format.eg{"command": "led1:1,lcd hello"}'
        }
    ]
    )
    return (completion.choices[0].message.content)