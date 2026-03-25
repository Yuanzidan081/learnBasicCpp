# 编码：UTF-8  无乱码 | Windows MinGW Make 专用
# 支持：make msvc2022  /  make g++

# ==================== 清理缓存 ====================
clean-cmake:
	@rm -f ./build/CMakeCache.txt
	@rm -rf ./build/CMakeFiles

clean: clean-cmake

# ==================== 切换 MSVC 2022 ====================
msvc2022:
	@if [ -f ./build/CMakeCache.txt ]; then \
		if ! grep -q "Visual Studio" ./build/CMakeCache.txt; then \
			make clean-cmake; \
		fi \
	fi
	@echo "=== generate MSVC 2022 project ==="
	cmake -S . -B build -G "Visual Studio 17 2022"

# ==================== 切换 G++ ====================
g++:
	@if [ -f ./build/CMakeCache.txt ]; then \
		if ! grep -q "Unix Makefiles" ./build/CMakeCache.txt; then \
			make clean-cmake; \
		fi \
	fi
	@echo "=== generate g++ project ==="
	cmake -S . -B build -G "Unix Makefiles"

bb:
	@echo "=== build project ==="
	cmake --build build
