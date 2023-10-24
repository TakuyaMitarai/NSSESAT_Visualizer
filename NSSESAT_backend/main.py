from fastapi import FastAPI
from fastapi.middleware.cors import CORSMiddleware
from pydantic import BaseModel
from treeimagename import generate_tree_image
import subprocess
import base64

app = FastAPI()

# CORS設定
app.add_middleware(
    CORSMiddleware,
    allow_origins=["http://localhost:8080"],
    allow_credentials=True,
    allow_methods=["*"],
    allow_headers=["*"],
)


class PointData(BaseModel):
    clicked_point: str
@app.post("/compile_and_run_cpp")
async def compile_and_run_cpp():
    try:
        # C++のコンパイル
        subprocess.run(["g++-13", "*.cpp"])
        
        # コンパイル後の実行
        result = subprocess.run(["./a.out"], capture_output=True, text=True)
        
        return {"message": "Successfully compiled and executed", "output": result.stdout}
    except Exception as e:
        print(f"An error occurred: {e}")
        return {"message": f"An error occurred: {e}"}

@app.post("/set_point")
async def set_point(point_data: PointData):
    imagefilename = point_data.clicked_point
    print("Received imagefilename:", imagefilename)
    try:
        generate_tree_image(imagefilename)  # imagefilenameを渡す
        with open(f"output.png", "rb") as img_file:  # 出力ファイル名も変更
            img_base64 = base64.b64encode(img_file.read()).decode("utf-8")
        return {"message": "Data received", "image": img_base64}
    except Exception as e:
        print(f"An error occurred while generating the tree image: {e}")
        return {"message": f"An error occurred: {e}"}


@app.get("/get_data")
async def get_data():
    with open('data.txt', 'r') as f:
        lines = f.readlines()
    data_points = []
    for line in lines:
        x, y = line.strip().split()
        data_points.append({"x": float(x), "y": int(y)})
    return {"plotdata": data_points}
