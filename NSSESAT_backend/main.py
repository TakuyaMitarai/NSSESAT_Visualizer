from fastapi import FastAPI, Request, UploadFile, File
from fastapi.middleware.cors import CORSMiddleware
from pydantic import BaseModel
from genmax import update_generation_max
from treeimagename import generate_tree_image
from fileedit import fileedit
from pathlib import Path
import subprocess
import base64
import os
import glob
import random

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

def shuffle_data(file_path, shuffle_times=1):
    # ファイルパスの生成
    full_path = file_path

    # ファイルの読み込み
    with open(full_path, 'r') as file:
        # スペース区切りでデータを読み込み、リストに格納
        data = [line.strip().split(' ') for line in file]

    # データのシャッフル
    # 指定された回数だけシャッフルを実行
    for _ in range(shuffle_times):
        random.shuffle(data)

    # ファイルへの上書き
    with open(full_path, 'w') as file:
        for line in data:
            # スペース区切りでデータを書き込み
            file.write(' '.join(line) + '\n')

@app.post("/uploadfile/")
async def upload_file(file: UploadFile = File(...)):
    with open(Path.cwd() / "dataset.txt", "wb") as buffer:
        buffer.write(file.file.read())
    fileedit()
    shuffle_data('./nssesat/Data/data.txt', shuffle_times=1)
    return {"filename": file.filename}


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
        cmd_compile = ["g++-13", "-O2"] + cpp_files
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

    # 数値1と数値2を取得
    value1, value2 = map(float, imagefilename.split('-'))
    value2 = value2 * 100
    value2 = round(value2, 3)

    try:
        generate_tree_image(imagefilename)  # imagefilenameを渡す
        with open(f"output.png", "rb") as img_file:  # 出力ファイル名も変更
            img_base64 = base64.b64encode(img_file.read()).decode("utf-8")
        return {"message": "Data received", "image": img_base64, "誤り率": value1, "ノード数": value2}
    except Exception as e:
        print(f"An error occurred while generating the tree image: {e}")
        return {"message": f"An error occurred: {e}"}

@app.get("/get_data")
async def get_data():
    return {"plotdata": read_data('result.txt')}

@app.get("/get_data2")
async def get_data2():
    return {"plotdata": read_data('result2.txt')}

def read_data(filename):
    with open(filename, 'r') as f:
        lines = f.readlines()
    data_points = []
    for line in lines:
        x, y = line.strip().split()
        data_points.append({"x": int(x), "y": float(y)})
    return data_points