# treeimagename.py

import re
import subprocess

def generate_tree_image(imagefilename):
    column_names = []
    with open("./nssesat/name/name.txt", "r") as name_file:
        for line in name_file:
            column_names.extend(line.strip().split())
            
    dot_file_path = f"nssesat/treedescription/treedata/{imagefilename}.dot"  # 動的に.dotファイルのパスを設定
    png_output_path = "output.png"  # 出力ファイル名は変更しない

    with open(dot_file_path, "r") as f:  # 動的な.dotファイル名で開く
        dot_data = f.read()

    pattern = r'label="(\d+)"(?![<])'

    def replacer(match):
        index = int(match.group(1))
        if index < len(column_names):
            return f'label="{column_names[index]}"'
        else:
            return match.group(0)

    new_dot_data = re.sub(pattern, replacer, dot_data)

    with open("new_tree.dot", "w") as f:  # この出力ファイル名は変更しない
        f.write(new_dot_data)

    result = subprocess.run(['dot', '-Tpng', '-Gdpi=300', '-o', png_output_path, 'new_tree.dot'], capture_output=True, text=True)
    print("STDOUT:", result.stdout)
    print("STDERR:", result.stderr)
