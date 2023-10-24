def update_generation_max(gen):
    # ステップ 2: parameters.txt ファイルを読み込む
    with open("./nssesat/parameters.txt", "r") as f:
        lines = f.readlines()

    # ステップ 3: GenerationMax の値を更新する
    for i, line in enumerate(lines):
        if line.startswith("GenerationMax"):
            lines[i] = f"GenerationMax\t{gen}\n"
            break

    # ステップ 4: 更新された内容を parameters.txt に書き戻す
    with open("./nsse/parameters.txt", "w") as f:
        f.writelines(lines)
