import subprocess
import sys
import pylint.lint
from pylint import epylint

linting_locations = ['riichiroyale/']
formatter_opts = ['--target-version=py38', '--color'] + linting_locations

lint_opts = ['--rcfile=pylintrc', '--output-format=colorized'] + linting_locations

def run_lint(ci=False):
  if ci:
    _ci_lint()
  else:
    _user_lint()

def _user_lint():
  pylint.lint.Run(lint_opts)

def _ci_lint():
  return epylint.py_run(''.join(linting_locations), return_std=True)

def main():
  CI = (sys.argv[1] == 'ci') if len(sys.argv) > 1 else False
  if not CI:
    subprocess.call(['python', '-m', 'black'] + formatter_opts)

  subprocess.call(['python', '-m', 'unittest', 'riichiroyale.test'])

  run_lint(ci=CI)


if __name__ == '__main__':
    main()