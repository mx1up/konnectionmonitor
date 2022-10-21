# norootforbuild

Name:          konnectionmonitor
Version:       0.2
Release:       0
Summary:       TCPView alternative for linux

Group:         Productivity/Networking/Diagnostic
License:       GPL-3.0+
URL:           http://mattiesworld.gotdns.org/weblog/category/coding-excursions/konnectionmonitor
Source:        %{name}-%{version}.tar.gz
BuildRoot:     %{_tmppath}/%{name}-%{version}-build
# BuildArch:      noarch

BuildRequires: hicolor-icon-theme
BuildRequires: update-desktop-files
BuildRequires: cmake
BuildRequires: libqt4-devel

#Requires:       

%description
stable version of KonnectionMonitor

%prep
%setup -q


%build
%define cmake_kde4(d:) \
  export CFLAGS=$RPM_OPT_FLAGS \
  export CXXFLAGS=$RPM_OPT_FLAGS \
  export LDFLAGS="-Wl,-Bsymbolic-functions $LDFLAGS" \
  test -n "`which icerun 2>/dev/null`" && icerun=" -DKDE4_SERIALIZE_TOOL=icerun" \
  bdir=. \
  %{-d:dir=%{-d*} \
  mkdir $dir \
  cd $dir \
  bdir=.. } \
  cmake -DCMAKE_SKIP_RPATH=ON \\\
      -DCMAKE_INSTALL_PREFIX=%{_prefix} \\\
       $icerun $bdir %* \
%{nil}
%cmake_kde4 -d builddir
make %{?_smp_mflags}
 
%install
cd builddir
%{__make} DESTDIR=%{buildroot} install/strip
%suse_update_desktop_file -n %{buildroot}%{_datadir}/applications/konnectionmonitor.desktop
 
%clean
rm -rf %{buildroot}
 
 
%files
%defattr(-,root,root,-)
%{_bindir}/%{name}
%{_datadir}/applications/konnectionmonitor.desktop
%{_datadir}/icons/*/*/apps/konnectionmonitor.png


%changelog
