# NSSESAT_Visualizer
<img width="1800" alt="0 05:0 01" src="https://github.com/TakuyaMitarai/NSSESAT_Visualizer/assets/106578561/ea5aee9e-8043-4da3-ba56-f6d8931f5ee6">

本システムは10-fold交差検証の結果を示している．
- **緑**：訓練事例を適用させた最終世代のモデル
- **ピンク**：テスト事例を適用させた最良モデル
- **青**：各ノードにおける最良モデルの平均正答率

データセットはスペース区切りで一行目は属性名1, 2, 3, ... ,クラス名，以降の行は属性値1, 2, 3, ... ,クラス値(整数）を入れる．
サンプルデータセットはNSSESAT_backend/dataset.txt
