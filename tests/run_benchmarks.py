from pathlib import Path
import subprocess as sp
import datetime

TEST_DIR = Path(__file__).parent
CLOX_DIR = TEST_DIR.parent

CLOX_EXE = next(CLOX_DIR.glob("clox"))

def main():
    scripts = TEST_DIR.glob('benchmark/*.lox')
    before = datetime.datetime.now()
    for script in scripts:
        sp.run([CLOX_EXE, str(script)], capture_output=True, shell=False)
    after = datetime.datetime.now()
    print(f'took {after - before}')

if __name__ == "__main__":
    main()
