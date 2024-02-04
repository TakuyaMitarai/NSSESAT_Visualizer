import os

# ディレクトリが存在しなければ作成（親ディレクトリも含めて）
target_dir = "treedata"  # この部分は実際の相対パスまたは絶対パスに置き換えてください。
if not os.path.exists(target_dir):
    os.makedirs(target_dir)

# ファイルパスを定義
file_path1 = "../../result.txt"
file_path2 = "../../result2.txt"
output_file_path = "../../result3.txt"
with open(file_path1, "r") as file1:
        data1 = file1.read()

# 2番目のファイルを読み込む
with open(file_path2, "r") as file2:
    data2 = file2.read()

# 結合した内容を新しいファイルに書き出す
with open(output_file_path, "w") as outfile:
    # 最初のファイルの内容を書き出す
    outfile.write(data1)
    # 2番目のファイルの内容を書き出す
    outfile.write(data2)


# treedotfile.txt を読み込む
with open("../treedotfile.txt", "r") as f:
    treedotfile_lines = f.readlines()

# accnode.txt を読み込む
with open("../../result3.txt", "r") as f:
    accnode_lines = f.readlines()

print(accnode_lines)
# 各木と対応する accnode データを読み込む
tree_data = []
current_tree = []
skip_number_line = True
for line in treedotfile_lines:
    if skip_number_line:
        skip_number_line = False
        continue

    if line.strip() == "}":
        current_tree.append("}")  # 閉じる波括弧を追加
        tree_data.append(current_tree)
        current_tree = []
        skip_number_line = True
    else:
        current_tree.append(line.strip())

acc_data = [line.strip().split() for line in accnode_lines]

# 出力ファイルを作成

for filename in os.listdir("./treedata"):
        os.remove(os.path.join("./treedata", filename))

for i, (tree, acc) in enumerate(zip(tree_data, acc_data)):
    node_value, acc_value = acc
    filename = f"{node_value}-{acc_value}.dot" if acc_value and node_value else f"{i}.dot"
    filepath = os.path.join("treedata", filename)
    with open(filepath, "w") as f:
        f.write("\n".join(tree))

import random

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

# 使用例
shuffle_data('../Data/data.txt', shuffle_times=1)

print("Completed!")
