from fastapi import FastAPI
from fastapi.middleware.cors import CORSMiddleware
from pydantic import BaseModel
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
async def compile_and_run_cpp():
    try:
        # カレントディレクトリを変更
        os.chdir("nssesat")
        
        # コンパイル対象のC++ファイルを見つける
        cpp_files = glob.glob('*.cpp')
        cmd_compile = ["g++-13"] + cpp_files
        
        # C++のコンパイル
        subprocess.run(cmd_compile)
        
        # カレントディレクトリを元に戻す
        os.chdir("..")
        
        # C++コンパイル後の実行
        result_cpp = subprocess.run(["./a.out"], cwd="./nssesat", stdout=subprocess.PIPE, stderr=subprocess.PIPE)

        # Pythonスクリプトの実行（C++の実行が成功した場合）
        if result_cpp.returncode == 0:
            result_py = subprocess.run(["python3", "treesplit.py"], cwd="./nssesat/treedescription", stdout=subprocess.PIPE, stderr=subprocess.PIPE)
            print(f"Python script return code: {result_py.returncode}")
            print(f"Have {len(result_py.stdout)} bytes in stdout: {result_py.stdout}")
            print(f"Have {len(result_py.stderr)} bytes in stderr: {result_py.stderr}")

            if result_py.returncode == 0:
                return {"message": "Successfully compiled and executed both C++ and Python", "output_cpp": result_cpp.stdout, "output_py": result_py.stdout}
            else:
                return {"message": "Python script execution failed", "error_py": result_py.stderr}
        
        else:
            print(f"C++ Return code: {result_cpp.returncode}")
            print(f"Have {len(result_cpp.stdout)} bytes in stdout: {result_cpp.stdout}")
            print(f"Have {len(result_cpp.stderr)} bytes in stderr: {result_cpp.stderr}")
            return {"message": "C++ Compilation or execution failed", "error_cpp": result_cpp.stderr}

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
