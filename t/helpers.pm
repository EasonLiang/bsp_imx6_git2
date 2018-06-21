use File::Temp qw(tempdir); # in core since perl 5.6.1
use Linux::Clone; # neither in core nor in Debian :-/

my $retval = Linux::Clone::unshare Linux::Clone::NEWNS;
BAIL_OUT("Cannot unshare(NEWNS): $!") if $retval != 0;

sub bind_mount_tmp {
    my ($dir) = @_;
    my $tmp = tempdir(CLEANUP => 1);
    system("mount -n --bind $tmp $dir") == 0
        or BAIL_OUT("bind-mounting $tmp to $dir failed: $!");
    return $tmp;
}

# In a new mount namespace, bindmount tmpdirs on /etc/systemd,
# /lib/systemd/system, and /var/lib/systemd to start with clean state
# yet use the actual locations and code paths.  The test harnesses use
# systemctl which is linked to /lib/systemd/libsystemd-shared-$ver.so,
# thus do not bindmount a tmpdir on /lib/systemd.
sub test_setup() {
    unless ($ENV{'TEST_ON_REAL_SYSTEM'}) {
        my $etc_systemd = bind_mount_tmp('/etc/systemd');
        my $lib_systemd_system = bind_mount_tmp('/lib/systemd/system');
        my $var_lib_systemd = bind_mount_tmp('/var/lib/systemd');

        # Tell `systemctl` to do not speak with the world outside our namespace.
        $ENV{'SYSTEMCTL_INSTALL_CLIENT_SIDE'} = '1'
    }
}

# reads in a whole file
sub slurp {
    open my $fh, '<', shift;
    local $/;
    <$fh>;
}

sub state_file_entries {
    my ($path) = @_;
    my $bytes = slurp($path);
    return split("\n", $bytes);
}

my $dsh = "$FindBin::Bin/../script/deb-systemd-helper";

sub _unit_check {
    my ($unit_file, $cmd, $cb, $verb) = @_;

    my $retval = system("DPKG_MAINTSCRIPT_PACKAGE=test $dsh $cmd '$unit_file'");
    isnt($retval, -1, 'deb-systemd-helper could be executed');
    ok(!($retval & 127), 'deb-systemd-helper did not exit due to a signal');
    $cb->($retval >> 8, 0, "random unit file '$unit_file' $verb $cmd");
}

sub is_enabled { _unit_check($_[0], 'is-enabled', \&is, 'is') }
sub isnt_enabled { _unit_check($_[0], 'is-enabled', \&isnt, 'isnt') }

sub is_debian_installed { _unit_check($_[0], 'debian-installed', \&is, 'is') }
sub isnt_debian_installed { _unit_check($_[0], 'debian-installed', \&isnt, 'isnt') }

1;
