import os
import pandas as pd

def fileedit():
    # 1. dataset.txt を pandas DataFrame として読み込む
    df = pd.read_csv("dataset.txt", delim_whitespace=True, header=None)
    
    # 2. ./nssesat/Data ディレクトリ内のファイルを削除する
    for filename in os.listdir("./nssesat/Data"):
        os.remove(os.path.join("./nssesat/Data", filename))
    
    # 3. カラム数、目的変数の種類数、データ数を計算する
    column_names = df.iloc[0, :-1].tolist()  # 最初の行の最後の列を除外してカラム名を取得
    num_columns = len(column_names)
    target_values = df.iloc[1:, -1].unique()  # 最初の行を除外して目的変数の種類を取得
    num_target_values = len(target_values)
    num_data_rows = len(df) - 1  # 最初の一行を除外
    
    # data.info にカラム数、目的変数の種類数、データ数を保存
    with open("./nssesat/Data/data.info", "w") as f:
        f.write(f"{num_columns} {num_target_values} {num_data_rows}")

    # 最初の一行のみを除外したデータを data.txt として保存
    df.iloc[1:].to_csv("./nssesat/Data/data.txt", sep=" ", header=False, index=False)

    # 最初の一行目のカラム名（目的変数名を除く）を name.txt として保存
    with open("./nssesat/name/name.txt", "w") as f:
        f.write(" ".join(map(str, column_names)))
