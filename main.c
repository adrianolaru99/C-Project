#include <stdio.h>
#include <stdlib.h>
void XORSHIFT32(unsigned int **v,int w,int h,unsigned int s)
{
    *v=(unsigned int *)calloc(2*w*h,sizeof(unsigned int));
    (*v)[0]=s;
    int i;
    for(i=1; i<=2*w*h-1; i++)
    {
        s=s^(s<<13);
        s=s^(s>>17);
        s=s^(s<<5);
        (*v)[i]=s;
    }
}
void INCARCAREIMAGINE(unsigned char **header,unsigned char **picture,char *cale)
{
    FILE *fin;
    fin=fopen(cale,"rb");
    *header=(unsigned char *)calloc(54,sizeof(unsigned char));
    fread(*header,sizeof(unsigned char),54,fin);
    unsigned int latime=*(unsigned int *)&(*header)[18];
    unsigned int inaltime=*(unsigned int *)&(*header)[22];
    unsigned int padding=4-(3*(latime))%4;
    *picture=(unsigned char *)calloc((3*latime+padding)*inaltime,sizeof(unsigned char));
    int i,j;
    fread(*picture,sizeof(unsigned char),(3*latime+padding)*inaltime,fin);
    fclose(fin);
}
void AFISAREIMAGINE(unsigned char *header,unsigned char *picture,char *cale)
{
    FILE *fout;
    fout=fopen(cale,"wb");
    fwrite(header,sizeof(unsigned char),54,fout);
    unsigned int inaltime=*(unsigned int *)&(header[18]);
    unsigned int latime=*(unsigned int *)&(header[22]);
    unsigned int padding=4-(3*(latime%4))%4;
    fwrite(picture,sizeof(unsigned char),(3*latime+padding)*inaltime,fout);
    fclose(fout);
}
void FARAPADDING(unsigned char *picture,unsigned char **picturefinal,unsigned char *header)
{
    unsigned int inaltime=*(unsigned int *)&header[18];
    unsigned int latime=*(unsigned int *)&header[22];
    unsigned int padding=(4-(3*(latime))%4)%4;
    int k=-1,z=-1;
    int i,j;
    *picturefinal=(unsigned char *)calloc(3*latime*inaltime,sizeof(unsigned char));
    for(i=0; i<inaltime; i++)
    {
        for(j=0; j<3*latime+padding; j++)
        {
            z++;
            if(j<3*latime)
            {
                k++;
                (*picturefinal)[k]=picture[z];
            }
        }
    }
}
void GENERAREPERMUTARE(int w,int h,unsigned int *v,unsigned int **z)
{
    *z=(unsigned int *)calloc(w*h,sizeof(unsigned int));
    unsigned int size=w*h;
    int i;
    for(i=0; i<size; i++)
        (*z)[i]=i;

    for(i=size-1; i>1; i--)
    {
        unsigned int aux=v[size-1]%(i+1);
        unsigned int aux1;
        aux1=(*z)[aux];
        (*z)[aux]=(*z)[i];
        (*z)[i]=aux1;
    }
}
void PERMUTARE(unsigned char *header,unsigned char *picturefinal,unsigned char **picturefinall, int *z)
{
    int i,j;
    unsigned int latime=*(unsigned int *)&header[18];
    unsigned int lungime=*(unsigned int *)&header[22];
    *picturefinall=(unsigned char *)calloc(3*latime*lungime,sizeof(unsigned char));
    for(i=0; i<latime*lungime; i++)
    {
        for(j=0; j<3; j++)
        {
            unsigned int aux1=3*i+j;
            unsigned int f=z[i]*3+j;
            (*picturefinall)[f]=picturefinal[aux1];
        }
    }
}
void XORCRIPTARE(unsigned int w,unsigned int l,unsigned char *picturefinal,unsigned int *v,unsigned int sv)
{
    unsigned int f=((1<<16))-1,g=((1<<24)-1),h=(1<<8)-1,fsv=(1<<16)-1,gsv=(1<<24)-1,hsv=(1<<8)-1;
    h=h&v[w*l];
    f=f&v[w*l+1];
    f=f>>8;
    g=g&v[w*l+2];
    g=g>>16;
    hsv=hsv&sv;
    fsv=fsv&sv;
    fsv=fsv>>8;
    gsv=gsv&sv;
    gsv=gsv>>16;
    picturefinal[0]=picturefinal[0]^hsv^h;
    picturefinal[1]=picturefinal[1]^fsv^f;
    picturefinal[2]=picturefinal[2]^gsv^g;
    int i;
    for(i=3; i<3*l*w; i=i+3)
    {
        unsigned int f=((1<<16))-1,g=((1<<24)-1),h=(1<<8)-1;
        h=h&v[w*l+i/3];
        f=f&v[w*l+i/3];
        f=f>>8;
        g=g&v[w*l+i/3];
        g=g>>16;
        picturefinal[i]=picturefinal[i]^h^picturefinal[i-3];
        picturefinal[i+1]=picturefinal[i+1]^f^picturefinal[i-2];
        picturefinal[i+2]=picturefinal[i+2]^g^picturefinal[i-1];
    }
}
void DECRPITAREXOR(unsigned char *picturefinal,unsigned int w,unsigned int l,int sv,unsigned int *v)
{
    int i;
    for(i=3*l*w-1; i>2; i=i-3)
    {
        unsigned int f=((1<<16)-1),g=((1<<24)-1),h=(1<<8)-1;
        h=h&v[w*l+i/3];
        f=f&v[w*l+i/3];
        f=f>>8;
        g=g&v[w*l+i/3];
        g=g>>16;
        picturefinal[i]=picturefinal[i]^g^picturefinal[i-3];
        picturefinal[i-1]=picturefinal[i-1]^f^picturefinal[i-4];
        picturefinal[i-2]=picturefinal[i-2]^h^picturefinal[i-5];
    }
    unsigned int f=((1<<16))-1,g=((1<<24)-1),h=(1<<8)-1,fsv=(1<<16)-1,gsv=(1<<24)-1,hsv=(1<<8)-1;
    h=h&v[w*l];
    f=f&v[w*l+1];
    f=f>>8;
    g=g&v[w*l+2];
    g=g>>16;
    hsv=hsv&sv;
    fsv=fsv&sv;
    fsv=fsv>>8;
    gsv=gsv&sv;
    gsv=gsv>>16;
    picturefinal[0]=picturefinal[0]^hsv^h;
    picturefinal[1]=picturefinal[1]^fsv^f;
    picturefinal[2]=picturefinal[2]^gsv^g;
}
void DECRPITAREPERMUTARE(unsigned char *picturefinal,unsigned int latime,unsigned int lungime,unsigned int *z,unsigned char **picturefinal1)
{
    unsigned int i,j;
    unsigned int *z1=(unsigned int *)calloc(latime*lungime,sizeof(unsigned int));
    *picturefinal1=(unsigned int *)calloc(latime*lungime,sizeof(unsigned int));
    for(i=0; i<lungime*latime; i++)
        z1[z[i]]=i;
    for(i=0; i<lungime*latime; i++)
    {
        for(j=0; j<3; j++)
        {
            unsigned int aux=i*3+j;
            unsigned int aux1=z1[i]*3+j;
            (*picturefinal1)[aux1]=picturefinal[aux];
        }
    }
}
void Hipatrattest(char *cale,double *hib,double *hig,double *hir)
{
    FILE *f1=fopen(cale,"rb");
    *hib=0;
    *hig=0;
    *hir=0;
    fseek(f1,0,SEEK_SET);
    unsigned char *header;
    header=(unsigned char *)calloc(54,sizeof(unsigned char));
    fread(header,sizeof(unsigned char),54,f1);
    unsigned int lungime=*(unsigned int *)&header[18];
    unsigned int latime=*(unsigned int *)&header[22];
    unsigned int size=lungime*latime;
    unsigned char *culoare=(unsigned char *)calloc(3*size,sizeof(unsigned char));
    fread(culoare,sizeof(unsigned char),3*size,f1);
    int i,j,k;
    double f=0;
    int f2=0;
    f=latime*lungime/256;
    for(i=0; i<=255; i++)
    {
        for(j=0; j<lungime; j++)
        {
            for(k=0; k<3*latime; k=k+3)
            {
                if(culoare[j*3*latime+k]==i)
                    f2++;
            }
        }
        *hir=(f2-f)*(f2-f)/f+(*hir);
        f2=0;
    }
    f2=0;
    for(i=0; i<=255; i++)
    {
        for(j=0; j<lungime; j++)
        {
            for(k=1; k<3*latime; k=k+3)
                if(culoare[j*3*latime+k]==i)
                    f2++;
        }
        *hig=(f2-f)*(f2-f)/f+(*hig);
        f2=0;
    }
    for(i=0; i<=255; i++)
    {
        for(j=0; j<lungime; j++)
        {
            for(k=2; k<3*latime; k=k+3)
                if(culoare[j*3*latime+k]==i)
                    f2++;
        }
        *hib=(f2-f)*(f2-f)/f+(*hib);
        f2=0;
    }
    fclose(f1);
}
int main()
{
    unsigned char *header,*picture,*picturefinal,*cale;
    unsigned char *cale1;
    unsigned char *cale2;
    cale=(unsigned char *)calloc(100,sizeof(unsigned char));
    cale1=(unsigned char *)calloc(100,sizeof(unsigned char));
    cale2=(unsigned char *)calloc(100,sizeof(unsigned char));
    scanf("%s",cale);
    scanf("%s",cale1);
    scanf("%s",cale2);
    double hib,hig,hir;
    INCARCAREIMAGINE(&header,&picture,cale);
    unsigned int latime=*(unsigned int *)&header[18];
    unsigned int lungime=*(unsigned int *)&header[22];
    FARAPADDING(picture,&picturefinal,header);
    int i;
    unsigned int s,*v,*z,sv,j;
    scanf("%u%u",&s,&sv);
    XORSHIFT32(&v,lungime,latime,s);
    FARAPADDING(picture,&picturefinal,header);
    GENERAREPERMUTARE(latime,lungime,v,&z);
    unsigned char *picturefinal1;
    PERMUTARE(header,picturefinal,&picturefinal1,z);
    XORCRIPTARE(latime,lungime,picturefinal1,v,sv);
    AFISAREIMAGINE(header,picturefinal1,cale2);
    Hipatrattest(cale1,&hib,&hig,&hir);
    printf("%f %f %f",hib,hig,hir);
    DECRPITAREXOR(picturefinal1,latime,lungime,sv,v);
    unsigned char *picturefinal2;
    DECRPITAREPERMUTARE(picturefinal1,latime,lungime,z,&picturefinal2);
    AFISAREIMAGINE(header,picturefinal2,cale1);
    free(picture);
    free(picturefinal1);
    free(picturefinal2);
    free(picturefinal);
    return 0;
}
