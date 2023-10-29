# 使用するベースイメージをホストマシンに合わせる
FROM node:20.8.1

# 環境変数でホストアドレスを指定
ENV HOST=0.0.0.0

# ワーキングディレクトリを設定
WORKDIR /app

# 依存関係ファイルをコピー
COPY package*.json ./

# 依存関係をインストール
RUN npm install

# ソースコードをコピー
COPY . .

# アプリケーションを起動
CMD ["npm", "run", "dev"]
