# norootforbuild

Name:          konnectionmonitor
Version:       0.3
Release:       0
Summary:       TCPView alternative for linux

Group:         Productivity/Networking/Diagnostic
License:       GPL-3.0+
URL:           http://mattiesworld.gotdns.org/weblog/category/coding-excursions/konnectionmonitor
Source:        %{name}-%{version}.tar.gz
BuildRoot:     %{_tmppath}/%{name}-%{version}-build
# BuildArch:      noarch

BuildRequires: cmake >= 3.1.0
BuildRequires: hicolor-icon-theme
BuildRequires: update-desktop-files
BuildRequires: cmake
BuildRequires: pkgconfig
BuildRequires: pkgconfig(Qt5Core) >= 5.5.0
BuildRequires: pkgconfig(Qt5Widgets) >= 5.5.0
BuildRequires: pkgconfig(Qt5Network) >= 5.5.0
BuildRequires:  update-desktop-files

Requires(post): update-desktop-files
Requires(postun): update-desktop-files

%description
stable version of KonnectionMonitor

%prep
%autosetup -p1


%build
%cmake
%make_build
#%cmake_kde4 -d builddir
#make %{?_smp_mflags}
 
%install
%make_install
 
%clean
rm -rf %{buildroot}
 
 
%files
%doc CHANGELOG README
%defattr(-,root,root,-)
%{_bindir}/%{name}
%{_datadir}/applications/konnectionmonitor.desktop
%{_datadir}/icons/*/*/apps/konnectionmonitor.png


%changelog
