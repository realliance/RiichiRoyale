import pylint.lint
from pylint import epylint

linting_locations = ['main.py', 'riichiroyale/']
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