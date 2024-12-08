-- Author: Luiz G. Mugnaini A. <luizmugnaini@gmail.com>

local start_time = os.time()

-- -----------------------------------------------------------------------------
-- Available command line options
-- -----------------------------------------------------------------------------

local options = {
    c          = { on = false, description = "Compile the C solutions."             },
    cpp        = { on = false, description = "Compile the C++ solutions."           },
    rust       = { on = false, description = "Compile the Rust solutions."          },
    typescript = { on = false, description = "Compile the Typescript solutions."    },
    msvc       = { on = false, description = "Compile with Visual Studio compiler." },
    clang      = { on = false, description = "Compile with Clang."                  },
    release    = { on = false, description = "Compile in release mode."             },
    help       = { on = false, description = "Print the help message."              },
}

-- -----------------------------------------------------------------------------
-- Collect command line arguments
-- -----------------------------------------------------------------------------

-- Collect build options.
local custom_flags_idx = nil
for i = 1, #arg do
    if arg[i] == "--" then
        custom_flags_idx = i + 1
        break
    end
    local opt_start_idx = string.find(arg[i], "[^-]")
    local opt = string.sub(arg[i], opt_start_idx)
    options[opt].on = true
end

-- Print the help message.
if options.help.on then
    print("Usage:\n    lua " .. arg[0] .. " [options]\n\nOptions:")
    for k, v in pairs(options) do
        print(string.format("    -%-15s" .. "%s", k, v.description))
    end
    return
end

-- -----------------------------------------------------------------------------
-- Operating system information
-- -----------------------------------------------------------------------------

local os_info = {
    windows = (package.config:sub(1, 1) == "\\"),
    linux = false,
}

if not os_info.windows then
    local handle = io.popen("uname")
    local result = handle:read("*a")
    handle:close()

    if string.match(result, "Linux") then
        os_info.linux = true
    else
        print("Where the fuck are you running this script from??? An apple? pfff pathetic...")
        return
    end
end

if os_info.windows then
    os_info.path_sep = "\\"
    os_info.silence_cmd = " > NUL 2>&1"
else
    os_info.path_sep = "/"
    os_info.silence_cmd = " > /dev/null 2>&1"
end

local os_file_info = {
    unix = {
        obj = ".o",
        exe = "",
        dll = ".so",
        lib = ".a",
        lib_prefix = "lib",
    },
    windows = {
        dbg = ".pdb",
        obj = ".obj",
        exe = ".exe",
        dll = ".dll",
        lib = ".lib",
        lib_prefix = "",
    },
}

local lua_exe = (type(jit) == "table") and "luajit" or "lua"

-- -----------------------------------------------------------------------------
-- Utility functions
-- -----------------------------------------------------------------------------

local function log_info(msg)
    print("\x1b[1;35m[AOC INFO]\x1b[0m " .. msg)
end

local function exec(cmd_str, quiet)
    if quiet then
        cmd_str = cmd_str .. os_info.silence_cmd
    else
        log_info("Executing: " .. cmd_str)
    end

    os.execute(cmd_str)
end

local function concat(arr, join, is_prefix, no_space)
    if #arr < 1 then
        return ""
    end

    if not join then
        join = ""
    end

    local space = " "
    if no_space then
        space = ""
    end

    local acc = arr[1] .. space
    for i = 2, #arr do
        if arr[i] and (arr[i] ~= "") then
            acc = acc .. join .. arr[i] .. space
        end
    end

    if is_prefix then
        acc = join .. acc -- Add the prefix to the start of the string
    end

    if not no_space then
        acc = string.sub(acc, 1, #acc - 1) -- Remove the extraneous space at the end.
    end

    return acc
end

local function make_path(arr)
    return concat(arr, os_info.path_sep, false, true)
end

local function get_script_dir()
    local info = debug.getinfo(1, "S").source:sub(2)
    return info:match("(.*)[/\\]")
end

-- -----------------------------------------------------------------------------
-- Project configuration
-- -----------------------------------------------------------------------------

local root_dir = get_script_dir()
if not root_dir then
    root_dir = "."
end

local out_dir = make_path({ ".", "build" })

local os_ext = os_info.windows and os_file_info.windows or os_file_info.unix

local compiler_flags = {
    clang = {
        opt_include          = "-I",
        opt_define           = "-D",
        opt_link_flags_start = "",
        opt_lib_link         = "-l",
        opt_lib_path         = "-L",
        opt_std              = "-std=",
        opt_out_exe          = "-o",
        flags_debug          = "-pedantic -Wall -Wextra -Wpedantic -Wuninitialized -Wconversion -Wnull-pointer-arithmetic -Wnull-dereference -Wformat=2 -Wpointer-arith -Wno-unsafe-buffer-usage -Wno-declaration-after-statement -Werror=implicit-function-declaration -Wno-unused-variable -Werror -g -O0 -fsanitize=address -fsanitize=pointer-compare -fsanitize=pointer-subtract -fsanitize=undefined -fstack-protector-strong -fsanitize=leak",
        flags_release        = "-O2",
    },
    msvc = {
        opt_include          = "-I",
        opt_define           = "-D",
        opt_link_flags_start = "/link",
        opt_lib_link         = "",
        opt_lib_path         = "/LIBPATH:",
        opt_std              = "/std:",
        opt_out_obj          = "/Fo",
        opt_out_exe          = "/Fe",
        opt_out_pdb          = "/Fd",
        flags_debug          = "-nologo /INCREMENTAL:NO -Oi -MP -FC -GF -GA -GR- /EHa- /W4 /Ob1 /Od /Oy- /Z7 /RTC1 /MTd /fsanitize=address",
        flags_release        = "-nologo /INCREMENTAL:NO /EHa- -GR- -O2 /MT",
    },
    clang_cl = {
        opt_include          = "-I",
        opt_define           = "-D",
        opt_link_flags_start = "/link",
        opt_lib_link         = "",
        opt_lib_path         = "/LIBPATH:",
        opt_std              = "/std:",
        opt_out_exe          = "-o",
        flags_debug          = "/INCREMENTAL:NO /EHa- -Wno-unsafe-buffer-usage -Wno-declaration-after-statement -Wno-null-pointer-subtraction -Wall -Wextra -Wconversion -Wuninitialized -Wnull-pointer-arithmetic -Wnull-dereference -Wformat=2 -Wno-pre-c11-compat -Ob0 /Od /Oy- /Z7 /RTC1 -g /MTd",
        flags_release        = "/INCREMENTAL:NO /EHa- -O2 /MT",
    },
}

local function get_compiler_name(c_or_cpp)
    if options.msvc.on then
        return "cl"
    end

    if c_or_cpp == "c" then
        return os_info.windows and "clang-cl" or "clang"
    else
        return  os_info.windows and "clang-cl" or "clang++"
    end
end

local function get_c_toolchain(c_or_cpp)
    local target_windows = os_info.windows or options.msvc.on

    -- Defaults per platform.
    local toolchain = target_windows and compiler_flags.msvc or compiler_flags.clang

    if options.clang.on then
        toolchain = target_windows and compiler_flags.clang_cl or compiler_flags.clang
    elseif options.msvc.on then
        toolchain = compiler_flags.msvc
    end

    toolchain.cc = get_compiler_name(c_or_cpp)

    return toolchain
end

-- -----------------------------------------------------------------------------
-- Execute build instructions
-- -----------------------------------------------------------------------------

local function prepare_output_target()
    local mkdir_cmd = os_info.windows and "mkdir" or "mkdir -p"
    exec(mkdir_cmd .. " " .. out_dir, true)
end

local function make_rust_solutions()
    local aocs = { "aoc21" }

    for _, aoc in ipairs(aocs) do
        local solution_path = make_path({ aoc, "main.rs" })
        local solution_exe = make_path({ out_dir, "rust_" .. aoc .. os_ext.exe })
        exec(string.format("rustc %s -o %s", solution_path, solution_exe)) 
    end
end

local function make_cpp_solutions()
    local tc = get_c_toolchain()
    local aocs = { "aoc23" }

    for _, aoc in ipairs(aocs) do
        local solution_path = make_path({ aoc, "main.cpp" })
        local solution_exe = make_path({ out_dir, "cpp_" .. aoc.. os_ext.exe })

        local out_obj_flag = ""
        if tc.cc == "cl" then
            out_obj_flag = tc.opt_out_obj .. make_path({ out_dir, "cpp_" .. aoc.. os_ext.obj })
        end

        local flags = options.release.on and tc.flags_release or tc.flags_debug
        if tc.cc == "clang-cl" then
            flags = flags .. " -Wno-c++98-compat -Wno-c++20-compat"
        end

        exec(concat({
            tc.cc,
            tc.opt_std .. "c++20",
            flags,
            out_obj_flag,
            tc.opt_out_exe .. solution_exe,
            solution_path,
        }))
    end
end

local function build_yoneda_lib(tc)
    local cc_flag
    if tc.cc == "clang-cl" or tc.cc == "clang" then
        cc_flag = "-clang"
    else 
        cc_flag = "-msvc"
    end

    exec(concat({
        lua_exe,
        make_path({ "yoneda", "build.lua" }),
        cc_flag,
        options.release.on and "-release" or "-debug",
        "--",
        "-DYO_ENABLE_ANSI_COLORS",
    }))
end

local function make_c_solutions()
    local tc = get_c_toolchain()
    build_yoneda_lib(tc)

    local aocs = { "aoc24" }

    for _, aoc in ipairs(aocs) do
        local solution_path = make_path({ aoc, "main.c" })
        local solution_exe = make_path({ out_dir, "c_" .. aoc.. os_ext.exe })

        local out_obj_flag = ""
        if tc.cc == "cl" then
            out_obj_flag = tc.opt_out_obj .. make_path({ out_dir, "c_" .. aoc.. os_ext.obj })
        end

        exec(concat({
            tc.cc,
            tc.opt_std .. "c11",
            options.release.on and tc.flags_release or tc.flags_debug,
            tc.opt_include .. make_path({ "yoneda", "include" }),
            tc.opt_define .. "YO_DEBUG",
            tc.opt_define .. "YO_ENABLE_ANSI_COLORS",
            out_obj_flag,
            tc.opt_out_exe .. solution_exe,
            solution_path,
            tc.opt_link_flags_start,
            tc.opt_lib_path .. out_dir,
            tc.opt_lib_link .. (os_info.windows and "yoneda.lib" or "libyoneda.a"),
        }))
    end
end

prepare_output_target()

if options.rust.on then
    make_rust_solutions()
end

if options.cpp.on then
    make_cpp_solutions()
end

if options.c.on then
    make_c_solutions()
end

local end_time = os.time()
log_info(string.format("Time elapsed: %.5f seconds", os.difftime(end_time, start_time)))
