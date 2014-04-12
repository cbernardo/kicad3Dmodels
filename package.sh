ARCHNAME=kicad3dmodels_src_$(date -u +"%Y%m%d").tar
DIRNAME=kicad3dmodels

git archive --prefix="${DIRNAME}/" -o "${ARCHNAME}" HEAD

if [ -e "${DIRNAME}" ]
    then
        rm -rfv "${DIRNAME}/*"
fi

if [ -e "docs/python/kc3d.pdf" ]
    then
        mkdir -p "${DIRNAME}/pdf"
        cp -v "docs/python/kc3d.pdf" "${DIRNAME}/pdf/kc3d.pdf"
fi

if [ -e "docs/vrml_notes/kicad_3d_vrml.pdf" ]
    then
        mkdir -p "${DIRNAME}/pdf"
        cp -v "docs/vrml_notes/kicad_3d_vrml.pdf" "${DIRNAME}/pdf/kicad_3d_vrml.pdf"
fi

if [ -e "docs/python/kc3d.pdf" ] || [ -e "docs/vrml_notes/kicad_3d_vrml.pdf" ]
    then
        tar -rvf "${ARCHNAME}" "${DIRNAME}"
fi

if [ -e "${DIRNAME}" ]
    then
        rm -rfv "${DIRNAME}"
fi

gzip "${ARCHNAME}"