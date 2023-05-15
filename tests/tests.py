import unittest
from pathlib import Path
import subprocess as sp
import xmlrunner
from sys import argv

TEST_DIR = Path(__file__).parent
CLOX_DIR = TEST_DIR.parent
TARGET_DIR = CLOX_DIR.joinpath('target')

CLOX_EXE = str(argv[1] if len(argv) > 1 else "clox")
CLOX_EXE = next(CLOX_DIR.glob(CLOX_EXE))

class BookTests(unittest.TestCase):
    pass

def main() -> None:
    test_cases = TEST_DIR.glob('book_tests/*/*.lox')
    case: Path
    for case in test_cases:
        test_name: str = f'test__{case.parent.stem}__{case.stem}'
        test = test_generator(case)
        setattr(BookTests, test_name, test)


def test_generator(case: Path):
    def test(self):
        expected: str = extract_expected(case)
        result = sp.run([CLOX_EXE, str(case)], capture_output=True, shell=False, text=True, timeout=1)
        self.assertEqual(expected, result.stdout, 
                         f"""
                         Expected to get:
                         {expected}
                         but got:
                         {result.stdout}""")

    return test


def extract_expected(path: Path)->str:
    result: str = ''
    with open(path) as f:
        for line in f.readlines():
            if '// expect: ' not in line:
                continue

            result += line.split('// expect: ')[1]

    return result

if __name__ == '__main__':
    main()
    with open(TARGET_DIR.joinpath('test_results.xml'), 'wb') as output:
        unittest.main(
            testRunner=xmlrunner.XMLTestRunner(output=output),
            failfast=False, buffer=False, catchbreak=False
        )
