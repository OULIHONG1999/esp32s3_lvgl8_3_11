from datetime import datetime

def pre_build_action(source, target, env):
    print("=== Running pre-build script ===")
    print("This script runs before the build process.")

    # 生成时间戳
    timestamp = datetime.now().strftime("%Y-%m-%d %H:%M:%S")
    print(f"Build started at: {timestamp}")

    # 这里可以添加其他任务，例如生成文件、清理临时文件等
    print("Pre-build tasks completed.\n")

# 注册预处理动作
Import("env")
env.AddPreAction("buildprog", pre_build_action)