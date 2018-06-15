Name:           geotagging
Version:        0.2.0
Release:        %(date +%Y%m%d%H)%{?dist}
Summary:        Photography geotagging tool
Group:          Applications/Internet
License:        GNU GPLv3
URL:            https://github.com/jmlich/geotagging
Source0:        https://github.com/jmlich/geotagging/archive/master.tar.gz#/%{name}-%{version}.tar.gz

BuildRequires:  desktop-file-utils
BuildRequires:  qt5-devel >= 5.10.0
BuildRequires:  qt5-linguist
BuildRequires:  exiv2-devel

%description
Photography geotagging tool

%prep
%setup -q -n %{name}-%{version}


%build
%{qmake_qt5} PREFIX=%{_prefix}
make %{?_smp_mflags}

%install
make INSTALL_ROOT=$RPM_BUILD_ROOT install

#desktop-file-install --dir $RPM_BUILD_ROOT/opt/%{name}/share/applications\
#      $RPM_BUILD_ROOT%{_datadir}/applications/%{name}.desktop

%files
#/opt/geotagging/*
%{_bindir}/geotagging


%changelog
* Fri Jun 15 2018 Jozef Mlich <imlich@fit.vutbr.cz> - 0.2.0-1
- initial packaging

