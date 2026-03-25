# 编码：UTF-8  无乱码 | Windows MinGW Make 专用
# 支持：make msvc2019 / make g++

# ==================== clean cache ====================
clean-cmake:
	@if [ -f build/CMakeCache.txt ]; then \
		rm -f build/CMakeCache.txt; \
	fi

clean: clean-cmake

# ==================== switch MSVC 2019 ====================
msvc2019:
	@if [ -f build/CMakeCache.txt ]; then \
		CURRENT_GENERATOR=$$(grep -E '^CMAKE_GENERATOR:INTERNAL=' build/CMakeCache.txt | cut -d'=' -f2-); \
		if [ "x$$CURRENT_GENERATOR" != "xVisual Studio 16 2019" ]; then \
			$(MAKE) clean-cmake; \
		fi; \
	fi
	@echo "=== generate MSVC 2019 project ==="
	cmake -S . -B build -G "Visual Studio 16 2019"

# ==================== switch G++ ====================
g++:
	@if [ -f build/CMakeCache.txt ]; then \
		CURRENT_GENERATOR=$$(grep -E '^CMAKE_GENERATOR:INTERNAL=' build/CMakeCache.txt | cut -d'=' -f2-); \
		if [ "x$$CURRENT_GENERATOR" != "xUnix Makefiles" ]; then \
			$(MAKE) clean-cmake; \
		fi; \
	fi
	@echo "=== generate g++ project ==="
	cmake -S . -B build -G "Unix Makefiles"

bb:
	@echo "=== build project ==="
	cmake --build build
