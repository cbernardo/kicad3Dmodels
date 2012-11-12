import kc3d
import kc3dconn

out = kc3d.ofstream()

x = kc3dconn.Genhdr()
t0 = kc3d.Transform()

kc3d.SetupVRML("testhdr_M_6x1_8mm.wrl", out)
x.setColors("../../mcad/colors/black.mat", "../../mcad/colors/gold.mat", "../../mcad/colors/gold.mat")
x.setCase(6, 1, 2.54, 2.54, 2.72, .72, 0.4)
x.setPins(1, 1, 2, 10, 0.64, 0.64, 0.64, 0.3, 0.8, 4, 0)
x.build(t0, "HDR_MALE_SP_6x1_8MM", out, 0);
out.close()

kc3d.SetupVRML("testhdr_FS_6x1_8mm.wrl", out)
x.setColors("../../mcad/colors/black.mat", "../../mcad/colors/tin.mat", "../../mcad/colors/gold.mat")
x.setCase(6, 1, 2.54, 2.54, 8, .72, 0.4)
x.setPins(1, 0, 2, 10, 0.64, 1.6, 0.72, 0.3, 0.8, 24, 0.5)
x.build(t0, "HDR_FEMALE_SP_6x1_8MM", out, 0)
out.close()

kc3d.SetupVRML("testhdr_FR_6x1_8mm.wrl", out)
x.setColors("../../mcad/colors/black.mat", "../../mcad/colors/tin.mat", "../../mcad/colors/gold.mat")
x.setCase(6, 1, 2.54, 2.54, 8, .72, 0.4)
x.setPins(0, 0, 2, 10, 0.64, 1.6, 0.96, 0.3, 0.8, 24, 0.1)
x.build(t0, "HDR_FEMALE_RP_6x1_8MM", out, 0);
out.close()


