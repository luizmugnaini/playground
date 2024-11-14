-- Author: Luiz G. Mugnaini A. <luizmugnaini@gmail.com>

if #arg < 2 then
    print(string.format("Usage: %s %s LEVEL PROBLEM_NAME", arg[-1], arg[0]))
    return
end

local path_sep = "\\"

local level   = arg[1]
local problem = arg[2]

local problem_name
local file_extension = string.sub(problem, -4)
if file_extension ~= ".cpp" then
    problem_name = problem
    problem = problem .. ".cpp"
else
    problem_name = string.sub(problem, 1, -4)
end

os.execute("mkdir build > NUL 2>&1")

local solution_path = level .. path_sep .. problem
local exe = "build" .. path_sep .. problem_name .. ".exe"
local cmd = string.format(
    "cl /std:c++20 /nologo /EHsc /WX /W4 /Z7 /Fo%s /Fe%s %s",
    "build" .. path_sep .. problem_name .. ".obj",
    exe,
    solution_path
)

print(string.format("[INFO] Compiling %s...\n[EXEC] %s", solution_path, cmd))
os.execute(cmd)

print(string.format("\n[INFO] Executing program %s...", exe))
os.execute(exe)
