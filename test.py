import sys
import unittest
import pylint.lint
from pylint import epylint
from riichiroyale.test import *

linting_locations = ['riichiroyale/']
opts = ['--rcfile=pylintrc', '--output-format=colorized'] + linting_locations

def run_lint(ci=False):
  if ci:
    _ci_lint()
  else:
    _user_lint()

def _user_lint():
  pylint.lint.Run(opts)

def _ci_lint():
  return epylint.py_run(''.join(linting_locations), return_std=True)

def main():
  #CI = sys.argv[1] == 'ci' if len(sys.argv) > 1 else False
  #run_lint(ci=CI)

  unittest.main()

if __name__ == '__main__':
    main()