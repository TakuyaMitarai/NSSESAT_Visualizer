import os

# ディレクトリが存在しなければ作成（親ディレクトリも含めて）
target_dir = "treedata"  # この部分は実際の相対パスまたは絶対パスに置き換えてください。
if not os.path.exists(target_dir):
    os.makedirs(target_dir)

# 以降のコードは同じ
# ...


# treedotfile.txt を読み込む
with open("../treedotfile.txt", "r") as f:
    treedotfile_lines = f.readlines()

# accnode.txt を読み込む
with open("../../result.txt", "r") as f:
    accnode_lines = f.readlines()

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

print("Completed!")
