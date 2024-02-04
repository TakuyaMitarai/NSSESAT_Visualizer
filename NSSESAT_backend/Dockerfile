# 使用するベースイメージ
FROM python:3.11.5

# graphvizとg++をインストール
RUN apt-get update && apt-get install -y graphviz g++

# ワーキングディレクトリを設定
WORKDIR /app

# 依存関係ファイルをコピー
COPY requirements.txt ./

# 依存関係をインストール
RUN pip install --no-cache-dir -r requirements.txt

# ソースコードをコピー
COPY . .

# アプリケーションを起動
CMD ["uvicorn", "main:app", "--reload", "--host", "0.0.0.0"]

