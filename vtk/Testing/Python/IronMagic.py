import vtk

vtkxdmf = 0
try:
  import libvtkXdmfPython
  vtkxdmf = libvtkXdmfPython
except:
  import vtkXdmfPython
  vtkxdmf = vtkXdmfPython
import sys

import string

tt = vtk.vtkTesting()
tt.AddArgument("-D")
tt.AddArgument(sys.argv[1])
tt.AddArgument("-V")
tt.AddArgument("Baseline/%s.png" % sys.argv[2])

if not vtkxdmf:
  print "Cannot load vtkXdmfPython"
  sys.exit()

Ren1 = vtk.vtkRenderer()
Ren1.SetBackground(0.33, 0.35, 0.43)
RenWin1 = vtk.vtkRenderWindow()
RenWin1.AddRenderer(Ren1)
RenWin1.SetSize(300, 300)

camera=Ren1.GetActiveCamera()
camera.SetPosition(33.5, 33.5, 267.038)
camera.SetFocalPoint(33.5, 33.5, 33.5)
camera.SetViewUp(0, 1, 0)
camera.SetViewAngle(30)
camera.SetClippingRange(168.866, 315.51)

pvTemp69 = vtkxdmf.vtkXdmfReader()
pvTemp69.SetFileName("%s/Data/Iron Protein.xmf" % tt.GetDataRoot())
pvTemp69.SetStride(1, 1, 1)
pvTemp69.UpdateInformation()
pvTemp69.SetDomainName("IronProtein")
pvTemp69.UpdateInformation()
pvTemp69.DisableAllGrids()
pvTemp69.EnableGrid("Iron00")
pvTemp69.EnableGrid("Iron01")
pvTemp69.EnableGrid("Iron02")
pvTemp69.EnableGrid("Iron03")
pvTemp69.EnableGrid("Iron04")
pvTemp69.EnableGrid("Iron05")
pvTemp69.EnableGrid("Iron06")
pvTemp69.EnableGrid("Iron07")
pvTemp69.UpdateInformation()

strides = [ 1, 1, 1 ]

if len(sys.argv) > 3:
  sstrides = string.split(sys.argv[3], " ")
  if len(sstrides) == 3:
    strides[0] = int(sstrides[0])
    strides[1] = int(sstrides[1])
    strides[2] = int(sstrides[2])
  print strides
  
pvTemp69.SetStride(strides)

pvTemp206 = vtk.vtkContourFilter()
pvTemp207 = vtk.vtkContourFilter()
pvTemp208 = vtk.vtkContourFilter()
pvTemp209 = vtk.vtkContourFilter()
pvTemp210 = vtk.vtkContourFilter()
pvTemp211 = vtk.vtkContourFilter()
pvTemp212 = vtk.vtkContourFilter()
pvTemp213 = vtk.vtkContourFilter()
pvTemp206.SetInput(pvTemp69.GetOutput(0))
pvTemp207.SetInput(pvTemp69.GetOutput(1))
pvTemp208.SetInput(pvTemp69.GetOutput(2))
pvTemp209.SetInput(pvTemp69.GetOutput(3))
pvTemp210.SetInput(pvTemp69.GetOutput(4))
pvTemp211.SetInput(pvTemp69.GetOutput(5))
pvTemp212.SetInput(pvTemp69.GetOutput(6))
pvTemp213.SetInput(pvTemp69.GetOutput(7))
pvTemp206.SelectInputScalars("scalars")
pvTemp207.SelectInputScalars("scalars")
pvTemp208.SelectInputScalars("scalars")
pvTemp209.SelectInputScalars("scalars")
pvTemp210.SelectInputScalars("scalars")
pvTemp211.SelectInputScalars("scalars")
pvTemp212.SelectInputScalars("scalars")
pvTemp213.SelectInputScalars("scalars")
pvTemp206.SetValue(0, 127.5)
pvTemp207.SetValue(0, 127.5)
pvTemp208.SetValue(0, 127.5)
pvTemp209.SetValue(0, 127.5)
pvTemp210.SetValue(0, 127.5)
pvTemp211.SetValue(0, 127.5)
pvTemp212.SetValue(0, 127.5)
pvTemp213.SetValue(0, 127.5)
pvTemp206.SetComputeNormals(1)
pvTemp207.SetComputeNormals(1)
pvTemp208.SetComputeNormals(1)
pvTemp209.SetComputeNormals(1)
pvTemp210.SetComputeNormals(1)
pvTemp211.SetComputeNormals(1)
pvTemp212.SetComputeNormals(1)
pvTemp213.SetComputeNormals(1)
pvTemp206.SetComputeGradients(0)
pvTemp207.SetComputeGradients(0)
pvTemp208.SetComputeGradients(0)
pvTemp209.SetComputeGradients(0)
pvTemp210.SetComputeGradients(0)
pvTemp211.SetComputeGradients(0)
pvTemp212.SetComputeGradients(0)
pvTemp213.SetComputeGradients(0)
pvTemp206.SetComputeScalars(0)
pvTemp207.SetComputeScalars(0)
pvTemp208.SetComputeScalars(0)
pvTemp209.SetComputeScalars(0)
pvTemp210.SetComputeScalars(0)
pvTemp211.SetComputeScalars(0)
pvTemp212.SetComputeScalars(0)
pvTemp213.SetComputeScalars(0)
pvTemp205 = vtk.vtkLookupTable()
pvTemp205.SetNumberOfTableValues(256)
pvTemp205.SetHueRange(0.6667, 0)
pvTemp205.SetSaturationRange(1, 1)
pvTemp205.SetValueRange(1, 1)
pvTemp205.SetTableRange(0, 255)
pvTemp205.SetVectorComponent(0)
pvTemp205.Build()
pvTemp215 = vtk.vtkGeometryFilter()
pvTemp215.SetInput(pvTemp206.GetOutput(0))
pvTemp231 = vtk.vtkPolyDataMapper()
pvTemp231.SetInput(pvTemp215.GetOutput())
pvTemp231.SetImmediateModeRendering(1)
pvTemp231.SetScalarRange(0, 1)
pvTemp231.UseLookupTableScalarRangeOn()
pvTemp231.SetScalarVisibility(1)
pvTemp231.SetScalarModeToUsePointFieldData()
pvTemp231.SelectColorArray("scalars")
pvTemp231.SetLookupTable( pvTemp205)
pvTemp232 = vtk.vtkActor()
pvTemp232.SetMapper(pvTemp231)
pvTemp232.GetProperty().SetRepresentationToSurface()
pvTemp232.GetProperty().SetInterpolationToGouraud()
pvTemp232.GetProperty().SetAmbient(0)
pvTemp232.GetProperty().SetDiffuse(1)
pvTemp232.GetProperty().SetSpecular(0)
pvTemp232.GetProperty().SetSpecularPower(1)
pvTemp232.GetProperty().SetSpecularColor(1, 1, 1)
Ren1.AddActor( pvTemp232)
pvTemp217 = vtk.vtkGeometryFilter()
pvTemp217.SetInput(pvTemp207.GetOutput(0))
pvTemp242 = vtk.vtkPolyDataMapper()
pvTemp242.SetInput(pvTemp217.GetOutput())
pvTemp242.SetImmediateModeRendering(1)
pvTemp242.SetScalarRange(0, 1)
pvTemp242.UseLookupTableScalarRangeOn()
pvTemp242.SetScalarVisibility(1)
pvTemp242.SetScalarModeToUsePointFieldData()
pvTemp242.SelectColorArray("scalars")
pvTemp242.SetLookupTable( pvTemp205)
pvTemp243 = vtk.vtkActor()
pvTemp243.SetMapper(pvTemp242)
pvTemp243.GetProperty().SetRepresentationToSurface()
pvTemp243.GetProperty().SetInterpolationToGouraud()
pvTemp243.GetProperty().SetAmbient(0)
pvTemp243.GetProperty().SetDiffuse(1)
pvTemp243.GetProperty().SetSpecular(0)
pvTemp243.GetProperty().SetSpecularPower(1)
pvTemp243.GetProperty().SetSpecularColor(1, 1, 1)
Ren1.AddActor( pvTemp243)
pvTemp219 = vtk.vtkGeometryFilter()
pvTemp219.SetInput(pvTemp208.GetOutput(0))
pvTemp253 = vtk.vtkPolyDataMapper()
pvTemp253.SetInput(pvTemp219.GetOutput())
pvTemp253.SetImmediateModeRendering(1)
pvTemp253.SetScalarRange(0, 1)
pvTemp253.UseLookupTableScalarRangeOn()
pvTemp253.SetScalarVisibility(1)
pvTemp253.SetScalarModeToUsePointFieldData()
pvTemp253.SelectColorArray("scalars")
pvTemp253.SetLookupTable( pvTemp205)
pvTemp254 = vtk.vtkActor()
pvTemp254.SetMapper(pvTemp253)
pvTemp254.GetProperty().SetRepresentationToSurface()
pvTemp254.GetProperty().SetInterpolationToGouraud()
pvTemp254.GetProperty().SetAmbient(0)
pvTemp254.GetProperty().SetDiffuse(1)
pvTemp254.GetProperty().SetSpecular(0)
pvTemp254.GetProperty().SetSpecularPower(1)
pvTemp254.GetProperty().SetSpecularColor(1, 1, 1)
Ren1.AddActor( pvTemp254)
pvTemp221 = vtk.vtkGeometryFilter()
pvTemp221.SetInput(pvTemp209.GetOutput(0))
pvTemp264 = vtk.vtkPolyDataMapper()
pvTemp264.SetInput(pvTemp221.GetOutput())
pvTemp264.SetImmediateModeRendering(1)
pvTemp264.SetScalarRange(0, 1)
pvTemp264.UseLookupTableScalarRangeOn()
pvTemp264.SetScalarVisibility(1)
pvTemp264.SetScalarModeToUsePointFieldData()
pvTemp264.SelectColorArray("scalars")
pvTemp264.SetLookupTable( pvTemp205)
pvTemp265 = vtk.vtkActor()
pvTemp265.SetMapper(pvTemp264)
pvTemp265.GetProperty().SetRepresentationToSurface()
pvTemp265.GetProperty().SetInterpolationToGouraud()
pvTemp265.GetProperty().SetAmbient(0)
pvTemp265.GetProperty().SetDiffuse(1)
pvTemp265.GetProperty().SetSpecular(0)
pvTemp265.GetProperty().SetSpecularPower(1)
pvTemp265.GetProperty().SetSpecularColor(1, 1, 1)
Ren1.AddActor( pvTemp265)
pvTemp223 = vtk.vtkGeometryFilter()
pvTemp223.SetInput(pvTemp210.GetOutput(0))
pvTemp275 = vtk.vtkPolyDataMapper()
pvTemp275.SetInput(pvTemp223.GetOutput())
pvTemp275.SetImmediateModeRendering(1)
pvTemp275.SetScalarRange(0, 1)
pvTemp275.UseLookupTableScalarRangeOn()
pvTemp275.SetScalarVisibility(1)
pvTemp275.SetScalarModeToUsePointFieldData()
pvTemp275.SelectColorArray("scalars")
pvTemp275.SetLookupTable( pvTemp205)
pvTemp276 = vtk.vtkActor()
pvTemp276.SetMapper(pvTemp275)
pvTemp276.GetProperty().SetRepresentationToSurface()
pvTemp276.GetProperty().SetInterpolationToGouraud()
pvTemp276.GetProperty().SetAmbient(0)
pvTemp276.GetProperty().SetDiffuse(1)
pvTemp276.GetProperty().SetSpecular(0)
pvTemp276.GetProperty().SetSpecularPower(1)
pvTemp276.GetProperty().SetSpecularColor(1, 1, 1)
Ren1.AddActor( pvTemp276)
pvTemp225 = vtk.vtkGeometryFilter()
pvTemp225.SetInput(pvTemp211.GetOutput(0))
pvTemp286 = vtk.vtkPolyDataMapper()
pvTemp286.SetInput(pvTemp225.GetOutput())
pvTemp286.SetImmediateModeRendering(1)
pvTemp286.SetScalarRange(0, 1)
pvTemp286.UseLookupTableScalarRangeOn()
pvTemp286.SetScalarVisibility(1)
pvTemp286.SetScalarModeToUsePointFieldData()
pvTemp286.SelectColorArray("scalars")
pvTemp286.SetLookupTable( pvTemp205)
pvTemp287 = vtk.vtkActor()
pvTemp287.SetMapper(pvTemp286)
pvTemp287.GetProperty().SetRepresentationToSurface()
pvTemp287.GetProperty().SetInterpolationToGouraud()
pvTemp287.GetProperty().SetAmbient(0)
pvTemp287.GetProperty().SetDiffuse(1)
pvTemp287.GetProperty().SetSpecular(0)
pvTemp287.GetProperty().SetSpecularPower(1)
pvTemp287.GetProperty().SetSpecularColor(1, 1, 1)
Ren1.AddActor( pvTemp287)
pvTemp227 = vtk.vtkGeometryFilter()
pvTemp227.SetInput(pvTemp212.GetOutput(0))
pvTemp297 = vtk.vtkPolyDataMapper()
pvTemp297.SetInput(pvTemp227.GetOutput())
pvTemp297.SetImmediateModeRendering(1)
pvTemp297.SetScalarRange(0, 1)
pvTemp297.UseLookupTableScalarRangeOn()
pvTemp297.SetScalarVisibility(1)
pvTemp297.SetScalarModeToUsePointFieldData()
pvTemp297.SelectColorArray("scalars")
pvTemp297.SetLookupTable( pvTemp205)
pvTemp298 = vtk.vtkActor()
pvTemp298.SetMapper(pvTemp297)
pvTemp298.GetProperty().SetRepresentationToSurface()
pvTemp298.GetProperty().SetInterpolationToGouraud()
pvTemp298.GetProperty().SetAmbient(0)
pvTemp298.GetProperty().SetDiffuse(1)
pvTemp298.GetProperty().SetSpecular(0)
pvTemp298.GetProperty().SetSpecularPower(1)
pvTemp298.GetProperty().SetSpecularColor(1, 1, 1)
Ren1.AddActor( pvTemp298)
pvTemp229 = vtk.vtkGeometryFilter()
pvTemp229.SetInput(pvTemp213.GetOutput(0))
pvTemp308 = vtk.vtkPolyDataMapper()
pvTemp308.SetInput(pvTemp229.GetOutput())
pvTemp308.SetImmediateModeRendering(1)
pvTemp308.SetScalarRange(0, 1)
pvTemp308.UseLookupTableScalarRangeOn()
pvTemp308.SetScalarVisibility(1)
pvTemp308.SetScalarModeToUsePointFieldData()
pvTemp308.SelectColorArray("scalars")
pvTemp308.SetLookupTable( pvTemp205)
pvTemp309 = vtk.vtkActor()
pvTemp309.SetMapper(pvTemp308)
pvTemp309.GetProperty().SetRepresentationToSurface()
pvTemp309.GetProperty().SetInterpolationToGouraud()
pvTemp309.GetProperty().SetAmbient(0)
pvTemp309.GetProperty().SetDiffuse(1)
pvTemp309.GetProperty().SetSpecular(0)
pvTemp309.GetProperty().SetSpecularPower(1)
pvTemp309.GetProperty().SetSpecularColor(1, 1, 1)
Ren1.AddActor( pvTemp309)


tt.SetRenderWindow(RenWin1)

if tt.RegressionTest(10) != 1:
  sys.exit(1)
