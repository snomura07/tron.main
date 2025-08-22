# ベースイメージ（C++17, OpenCV開発用）
FROM ubuntu:22.04

ENV DEBIAN_FRONTEND=noninteractive

# ビルド環境と必要ライブラリ
RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    git \
    pkg-config \
    libgtk-3-dev \
    libavcodec-dev \
    libavformat-dev \
    libswscale-dev \
    libtbb2 \
    libtbb-dev \
    libjpeg-dev \
    libpng-dev \
    libtiff-dev \
    libdc1394-22-dev \
    libv4l-dev \
    libopenexr-dev \
    libxvidcore-dev \
    libx264-dev \
    python3-dev \
    python3-numpy \
    x11-apps \
    && rm -rf /var/lib/apt/lists/*

# OpenCV ソース取得
WORKDIR /opt
RUN git clone --branch 4.10.0 https://github.com/opencv/opencv.git \
    && git clone --branch 4.10.0 https://github.com/opencv/opencv_contrib.git

# OpenCV ビルド
WORKDIR /opt/opencv/build
RUN cmake -D CMAKE_BUILD_TYPE=Release \
    -D CMAKE_INSTALL_PREFIX=/usr/local \
    -D OPENCV_EXTRA_MODULES_PATH=/opt/opencv_contrib/modules \
    -D BUILD_EXAMPLES=ON \
    -D WITH_QT=OFF \
    -D WITH_GTK=ON \
    -D WITH_OPENGL=ON \
    -D ENABLE_PRECOMPILED_HEADERS=OFF \
    .. \
    && make -j"$(nproc)" \
    && make install \
    && ldconfig

# 開発用ディレクトリ
WORKDIR /workspace

CMD ["/bin/bash"]
