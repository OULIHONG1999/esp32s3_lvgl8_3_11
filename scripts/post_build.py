from datetime import datetime

def post_build_action(source, target, env):
    print("=== Running post-build script ===")
    print("This script runs after the build process.")

    # 生成时间戳
    timestamp = datetime.now().strftime("%Y-%m-%d %H:%M:%S")
    print(f"Build finished at: {timestamp}")

    # 这里可以添加其他任务，例如计算文件校验和、复制文件等
    print("Post-build tasks completed.\n")

# 注册后处理动作
Import("env")
env.AddPostAction("buildprog", post_build_action)