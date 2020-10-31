import subprocess
import sys

linting_locations = ['riichiroyale/']
opts = ['--target-version=py38', '--color'] + linting_locations

def main():
  CI = (sys.argv[1] == 'ci') if len(sys.argv) > 1 else False
  if not CI:
    subprocess.call(['python', '-m', 'black'] + opts)

  subprocess.call(['python', '-m', 'unittest', 'riichiroyale.test'])

if __name__ == '__main__':
    main()