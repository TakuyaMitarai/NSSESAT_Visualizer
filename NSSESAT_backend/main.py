from fastapi import FastAPI, Request
from fastapi.middleware.cors import CORSMiddleware
from pydantic import BaseModel
from genmax import update_generation_max
from treeimagename import generate_tree_image
import subprocess
import base64
import os
import glob

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
async def compile_and_run_cpp(request: Request):
    try:
        payload = await request.json()
        gen_value = payload.get("gen", None)
        if gen_value is not None:
            print(f"Received gen value: {gen_value}")
            update_generation_max(gen_value)
        
        os.chdir("nssesat")
        cpp_files = glob.glob('*.cpp')
        cmd_compile = ["g++-13"] + cpp_files
        subprocess.run(cmd_compile)
        os.chdir("..")
        
        result_cpp = subprocess.run(["./a.out"], cwd="./nssesat", stdout=subprocess.PIPE, stderr=subprocess.PIPE)
        if result_cpp.returncode == 0:
            result_py = subprocess.run(["python3", "treesplit.py"], cwd="./nssesat/treedescription", stdout=subprocess.PIPE, stderr=subprocess.PIPE)
            if result_py.returncode == 0:
                return {"message": "Successfully compiled and executed both C++ and Python", "fetchRequired": True}
            else:
                return {"message": "Python script execution failed", "error_py": result_py.stderr}
        else:
            return {"message": "C++ Compilation or execution failed", "error_cpp": result_cpp.stderr}
    except Exception as e:
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
