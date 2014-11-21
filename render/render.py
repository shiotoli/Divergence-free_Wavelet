import os
import sys
import time
import gzip

if len(sys.argv)!=3 and len(sys.argv)!=2:
    print "usage: render_all.py start end\nto render frame start to end-1"
else:
    XRES = 300;
    YRES = 300;

    SAMPLES = 12; # preview
    #SAMPLES = 24; # high quality

    RENDER_SCRIPT = "scene.pbrt";


    template = file(RENDER_SCRIPT, "r").read()
    frame_start = int(sys.argv[1])
    if len(sys.argv)==2:  
		frame_end = frame_start+1;
    else:
		frame_end = int(sys.argv[2])
    for f in range(frame_start, frame_end):
        t1 = time.clock()
        print "Rendering Frame #%4d ..."%f
        scene = template.replace("##XRES##", str(XRES)).replace("##YRES##", str(YRES)).replace("##SAMPLES##", str(SAMPLES)).replace("##FRAMENUMBER##", "%04d"%f).replace("##SIZE##","small")
        filename = "render_script_%04d.pbrt"%f
        file(filename,"w").write(scene)
        
        dens_filename = "density_%04d.pbrt"%f
        dens_file = file(dens_filename,"w");
        dens_file.write(gzip.GzipFile("../pbrt/"+dens_filename+".gz").read())
        dens_file.close()
        
        os.system("pbrt %s"%filename)
        
        os.remove(filename)
        os.remove(dens_filename)
        """(pi,po) = os.popen2("pbrt")
        pi.write(scen)
        pi.flush()
        pi.close()
        po.close()"""
        t2 = time.clock()
        print "Done.", "%.2f sec"%(t2-t1)
