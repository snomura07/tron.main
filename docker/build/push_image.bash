#!/bin/bash

# 引数のチェック
if [ "$#" -ne 1 ]; then
    echo "Usage: $0 <image_name>"
    exit 1
fi

# 引数から変数を設定
IMAGE_NAME=$1
DOCKER_HUB_USERNAME=snomura07
TAG="latest"

# Dockerイメージにタグ付け
docker tag ${IMAGE_NAME}:${TAG} ${DOCKER_HUB_USERNAME}/${IMAGE_NAME}:${TAG}

# Docker Hubにイメージをpush
docker push ${DOCKER_HUB_USERNAME}/${IMAGE_NAME}:${TAG}

echo "Docker image pushed successfully to Docker Hub."
