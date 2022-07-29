Name:           com.github.jmlich.geotagging
Version:        0.6.1
Release:        %(date +%Y%m%d%H)%{?dist}
Summary:        Photography geotagging tool
Group:          Applications/Internet
License:        GNU GPLv3
URL:            https://github.com/jmlich/geotagging
Source0:        https://github.com/jmlich/geotagging/archive/master.tar.gz#/%{name}-%{version}.tar.gz

BuildRequires:  desktop-file-utils
BuildRequires:  libappstream-glib
BuildRequires:  exiv2-devel
BuildRequires:  qt6-qtbase-devel
BuildRequires:  qt6-qtwebchannel-devel
BuildRequires:  qt6-qttools-devel
BuildRequires:  git

%description
Photography geotagging tool

%prep
%setup -q -n geotagging-master

%build
%cmake -DVERSION:STRING=%{version}
%cmake_build


%install
%cmake_install
desktop-file-install --dir=${RPM_BUILD_ROOT}%{_datadir}/applications %{name}.desktop
appstream-util validate-relax --nonet %{buildroot}%{_metainfodir}/*.appdata.xml

%files
%{_bindir}/%{name}
%{_datadir}/applications/%{name}.desktop
%{_datadir}/icons/hicolor/86x86/apps/%{name}.png
%{_datadir}/icons/hicolor/128x128/apps/%{name}.png
%{_datadir}/%{name}/translation/*.qm
%{_metainfodir}/%{name}.appdata.xml

%changelog
* Sat Dec 04 2021 Jozef Mlich <jmlich83@gmail.com> - 0.6.1-1
- fix time zone handling while parsing of GPX

* Sat Dec 04 2021 Jozef Mlich <jmlich83@gmail.com> - 0.6.0-1
- Spanish (Latin America) translation - thanks to YoViajo
- fix of loading gpx with subsecond precision - thanks to Dmitry Saraev

* Fri Sep 10 2021 Jozef Mlich <imlich@fit.vutbr.cz> - 0.5.9-1
- French translations - thanks to LeJun
- Danish translations - thanks to Erik 'Clicketyclick' Bachmann

* Sun May 23 2021 Jozef Mlich <imlich@fit.vutbr.cz> - 0.5.8-1
- fix hill shading overlay map when changing base map
- use own user agent to comply OpenStreetMap usage policy

* Fri Mar 05 2021 Jozef Mlich <imlich@fit.vutbr.cz> - 0.5.7-1
- measuring distance of selected POI and cursor
- loading camera direction from exif

* Fri Jun 15 2018 Jozef Mlich <imlich@fit.vutbr.cz> - 0.2.0-1
- initial packaging

