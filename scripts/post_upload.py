from datetime import datetime

def post_upload_action(source, target, env):
    print("=== Running post-upload script ===")
    print("This script runs after the firmware upload process.")

    # 生成时间戳
    timestamp = datetime.now().strftime("%Y-%m-%d %H:%M:%S")
    print(f"Upload finished at: {timestamp}")

    # 这里可以添加其他任务，例如发送通知、记录日志等
    print("Post-upload tasks completed.\n")

# 注册后处理动作
Import("env")
env.AddPostAction("upload", post_upload_action)