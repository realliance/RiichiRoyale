import sys
import unittest

from lint import run_lint

def main():
  CI = sys.argv[1] == 'ci' if len(sys.argv) > 1 else False
  run_lint(ci=CI)

  unittest.main()

if __name__ == '__main__':
    main()