import os
import glob
env = Environment(ENV = os.environ)

env.Append(LIBS=["png","GL","GLU","glut"])
env.Append(CXXFLAGS=["-std=c++17","-g","-Wall","-O3","-I/usr/include/libpng12"])
env.Append(LINKFLAGS=["-L/usr/local/lib"])

env.Program("cloth_simulation",glob.glob("*.cpp"))