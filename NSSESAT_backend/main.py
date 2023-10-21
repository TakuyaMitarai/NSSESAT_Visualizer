from fastapi import FastAPI
from fastapi.middleware.cors import CORSMiddleware
import json

app = FastAPI()

# CORS設定
app.add_middleware(
    CORSMiddleware,
    allow_origins=["http://localhost:8080"],
    allow_credentials=True,
    allow_methods=["*"],
    allow_headers=["*"],
)

@app.get("/get_data")
async def get_data():
    with open('data.txt', 'r') as f:
        lines = f.readlines()
    data_points = []
    for line in lines:
        x, y = line.strip().split()
        data_points.append({"x": float(x), "y": int(y)})
    return {"plotdata": data_points}
