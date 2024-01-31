#!/bin/zsh

# バックエンドの起動
echo "Starting NSSESAT_backend..."
cd NSSESAT_backend
source env/bin/activate
uvicorn main:app --reload