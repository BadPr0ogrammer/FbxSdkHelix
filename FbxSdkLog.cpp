
#include "FbxSdkLog.h"

#include <iostream>
#include <sstream>
#include <iomanip>
#include <limits>
#include <numbers>

std::string FbxSdkLog::MetaDataConnections(FbxObject* pObject)
{
	std::string buf;
	int nbMetaData = pObject->GetSrcObjectCount<FbxObjectMetaData>();
	if (nbMetaData > 0)
		buf = "MetaData connections:";
	for (int i = 0; i < nbMetaData; i++)
	{
		FbxObjectMetaData* metaData = pObject->GetSrcObject<FbxObjectMetaData>(i);
		buf += (char*)metaData->GetName();
		buf += ";";
	}
	return buf;
}

std::string FbxSdkLog::ControlsPoints(FbxMesh* pMesh)
{
	int i, lControlPointsCount = pMesh->GetControlPointsCount();
	FbxVector4* lControlPoints = pMesh->GetControlPoints();
	std::stringstream ss;
	ss << "Control Points:";
	ss << std::fixed << std::setprecision(4);
	for (i = 0; i < 1/*lControlPointsCount*/; i++)
	{
		FbxVector4 p = lControlPoints[i];
		ss << i << ";" << p[0] << ";" << p[1] << ";" << p[2] << ";";
		for (int j = 0; j < pMesh->GetElementNormalCount(); j++)
		{
			FbxGeometryElementNormal* leNormals = pMesh->GetElementNormal(j);
			if (leNormals->GetMappingMode() == FbxGeometryElement::eByControlPoint)
			{
				ss << "Normal Vector:";
				if (leNormals->GetReferenceMode() == FbxGeometryElement::eDirect) {
					FbxVector4 v = leNormals->GetDirectArray().GetAt(i);
					ss << v[0] << ";" << v[1] << ";" << v[2] << ";";
				}
			}
		}
	}
	return ss.str();
}

std::string FbxSdkLog::Polygons(FbxMesh* pMesh)
{
	int i, j, lPolygonCount = pMesh->GetPolygonCount();
	FbxVector4* lControlPoints = pMesh->GetControlPoints();
	std::stringstream ss;
	ss << "Polygons";
	ss << std::fixed << std::setprecision(4);
	int vertexId = 0;
	for (i = 0; i < 1/*lPolygonCount*/; i++)
	{
		ss << i << ";";
		int l;
		for (l = 0; l < pMesh->GetElementPolygonGroupCount(); l++)
		{
			FbxGeometryElementPolygonGroup* lePolgrp = pMesh->GetElementPolygonGroup(l);
			switch (lePolgrp->GetMappingMode())
			{
			case FbxGeometryElement::eByPolygon:
				if (lePolgrp->GetReferenceMode() == FbxGeometryElement::eIndex)
				{
					ss << "Group Id:";
					int polyGroupId = lePolgrp->GetIndexArray().GetAt(i);
					ss << polyGroupId;
					break;
				}
			default:
				// any other mapping modes don't make sense
				ss << "unsupported group";
				break;
			}
		}
		int lPolygonSize = pMesh->GetPolygonSize(i);
		for (j = 0; j < lPolygonSize; j++)
		{
			int lControlPointIndex = pMesh->GetPolygonVertex(i, j);
			if (lControlPointIndex < 0)
			{
				ss << "Coordinates: Invalid index found!";
				continue;
			}
			else {
				ss << "Coordinates:";
				FbxVector4 v = lControlPoints[lControlPointIndex];
				ss << v[0] << ";" << v[1] << ";" << v[2] << ";";
			}
			for (l = 0; l < pMesh->GetElementVertexColorCount(); l++)
			{
				FbxGeometryElementVertexColor* leVtxc = pMesh->GetElementVertexColor(l);
				ss << "Color vertex:";
				switch (leVtxc->GetMappingMode())
				{
				default:
					break;
				case FbxGeometryElement::eByControlPoint:
					switch (leVtxc->GetReferenceMode())
					{
					case FbxGeometryElement::eDirect:
					{
						FbxColor c = leVtxc->GetDirectArray().GetAt(lControlPointIndex);
						ss << c.mRed << ";" << c.mGreen << ";" << c.mBlue << ";";
						break;
					}
					case FbxGeometryElement::eIndexToDirect:
					{
						int id = leVtxc->GetIndexArray().GetAt(lControlPointIndex);
						FbxColor c = leVtxc->GetDirectArray().GetAt(id);
						ss << c.mRed << ";" << c.mGreen << ";" << c.mBlue << ";";
						break;
					}
					default:
						break; // other reference modes not shown here!
					}
					break;
				case FbxGeometryElement::eByPolygonVertex:
				{
					switch (leVtxc->GetReferenceMode())
					{
					case FbxGeometryElement::eDirect:
					{
						FbxColor c = leVtxc->GetDirectArray().GetAt(vertexId);
						ss << c.mRed << ";" << c.mGreen << ";" << c.mBlue << ";";
						break;
					}
					case FbxGeometryElement::eIndexToDirect:
					{
						int id = leVtxc->GetIndexArray().GetAt(vertexId);
						FbxColor c = leVtxc->GetDirectArray().GetAt(id);
						ss << c.mRed << ";" << c.mGreen << ";" << c.mBlue << ";";
						break;
					}
					default:
						break; // other reference modes not shown here!
					}
				}
				break;
				case FbxGeometryElement::eByPolygon: // doesn't make much sense for UVs
				case FbxGeometryElement::eAllSame:   // doesn't make much sense for UVs
				case FbxGeometryElement::eNone:       // doesn't make much sense for UVs
					break;
				}
			}
			for (l = 0; l < pMesh->GetElementUVCount(); ++l)
			{
				FbxGeometryElementUV* leUV = pMesh->GetElementUV(l);
				ss << "Texture UV:";
				switch (leUV->GetMappingMode())
				{
				default:
					break;
				case FbxGeometryElement::eByControlPoint:
					switch (leUV->GetReferenceMode())
					{
					case FbxGeometryElement::eDirect:
					{
						FbxVector2 v = leUV->GetDirectArray().GetAt(lControlPointIndex);
						ss << v[0] << ";" << v[1] << ";";
						break;
					}
					case FbxGeometryElement::eIndexToDirect:
					{
						int id = leUV->GetIndexArray().GetAt(lControlPointIndex);
						FbxVector2 v = leUV->GetDirectArray().GetAt(id);
						ss << v[0] << ";" << v[1] << ";";
					}
					break;
					default:
						break; // other reference modes not shown here!
					}
					break;
				case FbxGeometryElement::eByPolygonVertex:
				{
					int lTextureUVIndex = pMesh->GetTextureUVIndex(i, j);
					switch (leUV->GetReferenceMode())
					{
					case FbxGeometryElement::eDirect:
					case FbxGeometryElement::eIndexToDirect:
					{
						FbxVector2 v = leUV->GetDirectArray().GetAt(lTextureUVIndex);
						ss << v[0] << ";" << v[1] << ";";
					}
					break;
					default:
						break; // other reference modes not shown here!
					}
				}
				break;
				case FbxGeometryElement::eByPolygon: // doesn't make much sense for UVs
				case FbxGeometryElement::eAllSame:   // doesn't make much sense for UVs
				case FbxGeometryElement::eNone:       // doesn't make much sense for UVs
					break;
				}
			}
			for (l = 0; l < pMesh->GetElementNormalCount(); ++l)
			{
				FbxGeometryElementNormal* leNormal = pMesh->GetElementNormal(l);
				ss << "Normal:";
				if (leNormal->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
				{
					switch (leNormal->GetReferenceMode())
					{
					case FbxGeometryElement::eDirect:
					{
						FbxVector4 v = leNormal->GetDirectArray().GetAt(vertexId);
						ss << v[0] << ";" << v[1] << ";" << v[2] << ";";
						break;
					}
					case FbxGeometryElement::eIndexToDirect:
					{
						int id = leNormal->GetIndexArray().GetAt(vertexId);
						FbxVector4 v = leNormal->GetDirectArray().GetAt(id);
						ss << v[0] << ";" << v[1] << ";" << v[2] << ";";
					}
					break;
					default:
						break; // other reference modes not shown here!
					}
				}
			}
			for (l = 0; l < pMesh->GetElementTangentCount(); ++l)
			{
				FbxGeometryElementTangent* leTangent = pMesh->GetElementTangent(l);
				ss << "Tangent:";
				if (leTangent->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
				{
					switch (leTangent->GetReferenceMode())
					{
					case FbxGeometryElement::eDirect:
					{
						FbxVector4 v = leTangent->GetDirectArray().GetAt(vertexId);
						ss << v[0] << ";" << v[1] << ";" << v[2] << ";";
						break;
					}
					case FbxGeometryElement::eIndexToDirect:
					{
						int id = leTangent->GetIndexArray().GetAt(vertexId);
						FbxVector4 v = leTangent->GetDirectArray().GetAt(id);
						ss << v[0] << ";" << v[1] << ";" << v[2] << ";";
					}
					break;
					default:
						break; // other reference modes not shown here!
					}
				}
			}
			for (l = 0; l < pMesh->GetElementBinormalCount(); ++l)
			{
				FbxGeometryElementBinormal* leBinormal = pMesh->GetElementBinormal(l);
				ss << "Binormal:";
				if (leBinormal->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
				{
					switch (leBinormal->GetReferenceMode())
					{
					case FbxGeometryElement::eDirect:
					{
						FbxVector4 v = leBinormal->GetDirectArray().GetAt(vertexId);
						ss << v[0] << ";" << v[1] << ";" << v[2] << ";";
						break;
					}
					case FbxGeometryElement::eIndexToDirect:
					{
						int id = leBinormal->GetIndexArray().GetAt(vertexId);
						FbxVector4 v = leBinormal->GetDirectArray().GetAt(id);
						ss << v[0] << ";" << v[1] << ";" << v[2] << ";";
					}
					break;
					default:
						break; // other reference modes not shown here!
					}
				}
			}
			vertexId++;
		} // for polygonSize
	} // for polygonCount
	return ss.str();
}
