@if exist personal.props goto NEXT
@copy tools\personal.props personal.props
:NEXT
@if exist thehell2.vNext.vcxproj.user goto NEXTER
@copy tools\thehell2.vNext.vcxproj.user thehell2.vNext.vcxproj.user
:NEXTER
@if exist thehell2.vcxproj.user goto END
@copy tools\thehell2.vcxproj.user thehell2.vcxproj.user
:END
