import os
import subprocess

# 初始化计数器
pass_count = 0
fail_count = 0

# 获取当前目录路径
current_dir = os.getcwd()

# 定义config目录路径
config_dir = os.path.join(current_dir, 'config')

# 确保config目录存在
if not os.path.exists(config_dir):
    print("config directory not found!")
    exit(1)

# 检查mtf.exe是否存在
mtf_exe_path = os.path.join(current_dir, "dirty_lens.exe")
if not os.path.exists(mtf_exe_path):
    print("dirty_lens.exe not found in the current directory!")
    exit(1)

# 遍历config目录，检索所有.ini文件
ini_files = [f for f in os.listdir(config_dir) if f.endswith('.ini')]

# 遍历每个.ini文件并执行mtf.exe
for ini_file in ini_files:
    ini_path = os.path.join(config_dir, ini_file)
    
    try:
        # 运行 mtf.exe XXX.ini
        result = subprocess.run([mtf_exe_path, ini_path], capture_output=True, text=True)

        # 打印命令行输出结果
        print(f"Output for {ini_file}:")
        print(result.stdout)  # 打印标准输出
        print(result.stderr)  # 打印标准错误输出

        # 获取输出结果
        output = result.stdout
        
        # 根据输出结果更新 pass 和 fail 计数器
        if "Lens is clean" in output:
            pass_count += 1
            print(f"{ini_file}: PASS")
        elif "Lens is dirty" in output:
            fail_count += 1
            print(f"{ini_file}: FAIL")
        else:
            print(f"{ini_file}: UNKNOWN RESULT")

    except Exception as e:
        print(f"Error running dirty_lens.exe with {ini_file}: {e}")

# 打印 pass 和 fail 的数量
print(f"\nSummary:")
print(f"Pass: {pass_count}")
print(f"Fail: {fail_count}")

# 暂停程序等待用户按键
input("Press Enter to exit...")
