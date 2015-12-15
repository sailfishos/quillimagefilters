Name:       quillimagefilter-qt5
Summary:    Qt based library for individual image processing operations
Version:    1.111202.0
Release:    0
Group:      System/Libraries
License:    LGPLv2.1 or GPLv3 or Qt Commercial or LGPLv2.1 with Nokia Qt LGPL Exception v1.1
URL:        https://git.merproject.org/mer-core/quillimagefilters
Source0:    %{name}-%{version}.tar.gz
Requires(post): /sbin/ldconfig
Requires(postun): /sbin/ldconfig
BuildRequires:  pkgconfig(quillmetadata-qt5)
BuildRequires:  pkgconfig(Qt5Core)
BuildRequires:  pkgconfig(Qt5Gui)
BuildRequires:  pkgconfig(Qt5Test)
BuildRequires:  libjpeg-devel

%description
A Qt based library which contains individual image loading, processing
and saving operations optimized for mobile devices. Operations are
used by libquill but can be also used directly if the high-level
engine framework of libquill is not desired.


%package tests
Summary:    Qt based image processing library - unit tests
Group:      Development/Libraries
Requires:   %{name} = %{version}-%{release}
Requires:   testrunner-lite
Requires(post): /sbin/ldconfig
Requires(postun): /sbin/ldconfig

%description tests
A Qt based library which contains individual image loading, processing
and saving operations optimized for mobile devices. Operations are
used by libquill but can be also used directly if the high-level
engine framework of libquill is not desired.

This package includes the unit tests.


%package benchmark
Summary:    Qt based image processing library - benchmark
Group:      Development/Libraries
Requires:   %{name} = %{version}-%{release}

%description benchmark
A Qt based library which contains individual image loading, processing
and saving operations optimized for mobile devices. Operations are
used by libquill but can be also used directly if the high-level
engine framework of libquill is not desired.

This package includes the benchmark.


%package devel
Summary:    Qt based image processing library - development headers
Group:      Development/Libraries
Requires:   %{name} = %{version}-%{release}

%description devel
A Qt based library which contains individual image loading, processing
and saving operations optimized for mobile devices. Operations are
used by libquill but can be also used directly if the high-level
engine framework of libquill is not desired.

This package includes the development headers.


%prep
%setup -q -n %{name}-%{version}

%build

%qmake5 

make %{?jobs:-j%jobs}

%install
rm -rf %{buildroot}
%qmake5_install

%post -p /sbin/ldconfig

%postun -p /sbin/ldconfig

%post tests -p /sbin/ldconfig

%postun tests -p /sbin/ldconfig

%files
%defattr(-,root,root,-)
%{_libdir}/libquillimagefilter-qt5.so*
%{_libdir}/qt5/plugins/quillimagefilters/*

%files tests
%defattr(-,root,root,-)
%{_libdir}/libut_unittests-qt5.so*
%{_libdir}/quillimagefilter-qt5-tests/*
%{_datadir}/quillimagefilter-qt5-tests/*

%files benchmark
%defattr(-,root,root,-)
%{_libdir}/quillimagefilter-qt5-benchmark/
%{_datadir}/quillimagefilter-qt5-benchmark/

%files devel
%defattr(-,root,root,-)
%{_includedir}/qt5/quillimagefilter-qt5/*
%{_libdir}/libquillimagefilter-qt5*.so
%{_libdir}/pkgconfig/quillimagefilter-qt5.pc

