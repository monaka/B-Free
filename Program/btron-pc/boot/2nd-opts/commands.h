HOOK_OPERATOR("fdinit",	init_fd)
HOOK_OPERATOR("fdreset", fd_reset)
HOOK_OPERATOR("ideid", ide_id)
HOOK_OPERATOR("ideinit", ide_init)
HOOK_OPERATOR("memory",	status_memory)

EXTN_OPERATOR("cat", f_sfs_cat)
EXTN_OPERATOR("dir", f_sfs_dir)
EXTN_OPERATOR("mount", f_sfs_mount)
EXTN_OPERATOR("sfsread", sfs_read_buf)
EXTN_OPERATOR("ideboot", ide_boot)
EXTN_OPERATOR("ideread", ide_read_dump)

TRAD_OPERATOR("reset", cpu_reset_func)
TRAD_OPERATOR("boot", boot_btron)
TRAD_OPERATOR("debug", debug_func)
TRAD_OPERATOR("dump", dump_func)
TRAD_OPERATOR("fdboot", boot_func)
TRAD_OPERATOR("fdread",	fd_read_func)
TRAD_OPERATOR("fdrecalib", fd_recalibrate_func)
TRAD_OPERATOR("fdseek",	seek_func)
TRAD_OPERATOR("fdstatus", fd_status_func)
TRAD_OPERATOR("help", help_func)
TRAD_OPERATOR("map", print_map_func)
TRAD_OPERATOR("run", run_func)
