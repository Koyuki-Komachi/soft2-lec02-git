Student、TStudent2では、アドレスが若い順にid,name,age,height,weightと並んでおり、ageのあとに空の４Bが付加されている。その結果、構造体のサイズは128Bになっている。
一方、TStudent1では、上の場合に加えて、weightのあとにtag
のメモリ領域があり、tagのあとに空の７Bが付加されていることで、構造体のサイズは136Bになっている。