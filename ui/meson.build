project('abalone_wk', 'cpp',
        version : '0.0.1',
        license: 'MIT'
    )
abalone_compiler_flag =  ['-rdynamic']
abalone_linker_flag = ['-Wl,--as-needed']
add_project_arguments([abalone_compiler_flag, abalone_linker_flag], language : 'cpp')
abalone_sources = ['abalone_ext.cpp']


webkit_dep = dependency('webkit2gtk-4.0', version: '>= 2.24.1')
webkit_web_extension_dep = dependency('webkit2gtk-web-extension-4.0', version: '>= 2.24.1')
glib_dep = dependency('glib-2.0', version: '>= 2.24.1')
lib_dep = [webkit_dep, webkit_web_extension_dep, glib_dep]
data_install_dir = join_paths(get_option('datadir'),'kapandaria/games/abalone')
extention_dir = join_paths(get_option('libdir') ,'kapandaria/games/abalone')
main_html_file = 'abalone_main.html'
icon_file = 'icon.png'
run_script = custom_target('abalonegame.sh',
  output : 'abalonegame.sh',
  command : [join_paths(meson.current_source_dir(),'create_run_script.sh'), join_paths(get_option('prefix'),extention_dir) , join_paths(get_option('prefix'),data_install_dir,main_html_file) , '"Abalone Game"', join_paths(get_option('prefix'),data_install_dir,icon_file) ,'@OUTPUT@'],
  install : true,
  install_dir : 'bin')

abalone_ext = shared_library('abaloneExt',
                                abalone_sources,
                                dependencies : lib_dep,
                                install: true,
                                install_dir : join_paths(get_option('libdir') ,'kapandaria/games/abalone'))
install_data([icon_file,'abalone.css','abalone_openning.svg', main_html_file, 'abalone_preferences.html'], install_dir : data_install_dir)
