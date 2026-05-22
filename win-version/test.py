import ctypes
import sys
import subprocess
import time
import os


def is_admin():
    try:
        return ctypes.windll.shell32.IsUserAnAdmin()
    except:
        return False
if __name__ == "__main__":

    if is_admin():
        script_dir = os.path.dirname(os.path.abspath(__file__))
        arg = 0x17DE34
        while True:
            target_program = os.path.join(script_dir, "cheat.exe")
            hex_arg = hex(arg)
            arguments = [hex_arg]
            print(f"====================================")
            print(f"{target_program} arg: {hex_arg}")
            process = subprocess.Popen([target_program] + arguments)
            time.sleep(5)
            process.kill()

            arg += 0x4

    else:
        script_args = " ".join([f'"{sys_arg}"' for sys_arg in sys.argv])
        ctypes.windll.shell32.ShellExecuteW(
            None, "runas", sys.executable, script_args, None, 1
        )