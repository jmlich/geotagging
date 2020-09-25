Name:           geotagging
Version:        0.5.3
Release:        %(date +%Y%m%d%H)%{?dist}
Summary:        Photography geotagging tool
Group:          Applications/Internet
License:        GNU GPLv3
URL:            https://github.com/jmlich/geotagging
Source0:        https://github.com/jmlich/geotagging/archive/master.tar.gz#/%{name}-%{version}.tar.gz

BuildRequires:  desktop-file-utils
BuildRequires:  libappstream-glib
BuildRequires:  exiv2-devel
BuildRequires:  qt5-qtwebengine-devel
BuildRequires:  qt5-qtbase-devel
BuildRequires:  qt5-qtquickcontrols
BuildRequires:  qt5-linguist
BuildRequires:  git

%description
Photography geotagging tool

%prep
%setup -q -n %{name}-%{version}

%build
%cmake
%cmake_build


%install
%cmake_install
desktop-file-install --dir=${RPM_BUILD_ROOT}%{_datadir}/applications %{name}.desktop
appstream-util validate-relax --nonet %{buildroot}%{_metainfodir}/*.appdata.xml

%files
%{_bindir}/geotagging
%{_datadir}/applications/%{name}.desktop
%{_datadir}/icons/hicolor/applications/64x64/geotagging.png
%{_datadir}/icons/hicolor/applications/64x64/geotagging.ico
%{_datadir}/geotagging/translation/*.qm
%{_metainfodir}/geotagging.appdata.xml

%changelog
* Fri Jun 15 2018 Jozef Mlich <imlich@fit.vutbr.cz> - 0.2.0-1
- initial packaging

