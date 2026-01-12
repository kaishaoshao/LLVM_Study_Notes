#!/bin/bash
# LLVM下载脚本 - 只需指定版本号

# 获取版本号（默认20.0.0）
VERSION="${1:-20.0.0}"
ARCHIVE="llvmorg-${VERSION}.tar.gz"
URL="https://github.com/llvm/llvm-project/archive/refs/tags/${ARCHIVE}"

echo "下载LLVM ${VERSION}..."

# 如果压缩包已存在，跳过下载
if [ -f "$ARCHIVE" ]; then
    echo "使用现有压缩包: $ARCHIVE"
else
    echo "⬇正在下载: $URL"
    # 尝试使用wget或curl
    if command -v wget >/dev/null 2>&1; then
        wget "$URL"
    elif command -v curl >/dev/null 2>&1; then
        curl -LO "$URL"
    else
        echo " 错误: 需要安装 wget 或 curl"
        exit 1
    fi
fi

# 解压
echo "📂 正在解压..."
if [ -f "$ARCHIVE" ]; then
    # 创建解压目录（llvm-版本号）
    EXTRACT_DIR="llvm-${VERSION}"
    mkdir -p "$EXTRACT_DIR"

    # 解压（自动去除顶层目录）
    tar -xzf "$ARCHIVE" --strip-components=1 -C "$EXTRACT_DIR" 2>/dev/null || \
    tar -xzf "$ARCHIVE" -C "$EXTRACT_DIR"  # 备选方案

    echo "🎉 完成！LLVM已解压到: $EXTRACT_DIR/"
    echo "💾 大小: $(du -sh "$EXTRACT_DIR" | cut -f1)"
else
    echo "❌ 错误: 下载失败，压缩包不存在"
    exit 1
fi