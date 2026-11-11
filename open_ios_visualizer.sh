#!/bin/sh
# Launch MemoryOS iOS-themed visualizer in browser
SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
HTML_PATH="$SCRIPT_DIR/web/index.html"

echo "========================================================"
echo "  MemoryOS - Apple Native iOS Memory Visualizer"
echo "========================================================"

if which xdg-open > /dev/null 2>&1; then
    xdg-open "$HTML_PATH"
elif which open > /dev/null 2>&1; then
    open "$HTML_PATH"
elif which wslview > /dev/null 2>&1; then
    wslview "$HTML_PATH"
else
    echo "Please open this file in your browser: file://$HTML_PATH"
fi
