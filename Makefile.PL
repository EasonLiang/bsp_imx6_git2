use inc::Module::Install;

name 'init-system-helpers';
version '1.0';

install_script 'deb-systemd-helper';

WriteAll;
