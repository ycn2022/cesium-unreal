// Copyright 2020-2021 CesiumGS, Inc. and Contributors

#pragma once

#include "Engine/StaticMesh.h"
#include "LocalVertexFactory.h"
#include "RHIResources.h"

/**
 * This generates the indices necessary for point attenuation in a
 * FCesiumGltfPointsComponent.
 */
class FCesiumPointAttenuationIndexBuffer : public FIndexBuffer {
public:
  FCesiumPointAttenuationIndexBuffer(const int32& NumPoints)
      : NumPoints(NumPoints) {}
  virtual void InitRHI() override;

private:
  // The number of points in the original point mesh. Not to be confused with
  // the number of vertices in the attenuated point mesh.
  const int32 NumPoints;
};

/**
 * This generates the vertices necessary for point attenuation in a
 * FCesiumGltfPointsComponent.
 */
class FCesiumPointAttenuationVertexBuffer : public FVertexBuffer {
public:
  FCesiumPointAttenuationVertexBuffer(const FPositionVertexBuffer* PositionVertexBuffer)
      : PositionVertexBuffer(PositionVertexBuffer) {}
  virtual void InitRHI() override;

  FShaderResourceViewRHIRef SRV;

private:
  const FPositionVertexBuffer* PositionVertexBuffer;
};

/**
 * The parameters to be passed as UserData to the
 * shader.
 */
struct FCesiumPointAttenuationBatchElementUserData {
  FRHIShaderResourceView* PackedTangentsBuffer;
  FRHIShaderResourceView* ColorBuffer;
  FRHIShaderResourceView* TexCoordBuffer;
  uint32 NumTexCoords;
  uint32 bHasPointColors;
  FVector3f AttenuationParameters;
};

class FCesiumPointAttenuationBatchElementUserDataWrapper
    : public FOneFrameResource {
public:
  FCesiumPointAttenuationBatchElementUserData Data;
};

class FCesiumPointAttenuationVertexFactory : public FLocalVertexFactory {

  DECLARE_VERTEX_FACTORY_TYPE(FCesiumPointAttenuationVertexFactory);

public:
  // Sets default values for this component's properties
  FCesiumPointAttenuationVertexFactory(
      ERHIFeatureLevel::Type InFeatureLevel,
      const FPositionVertexBuffer* PositionVertexBuffer);

  static bool ShouldCompilePermutation(
      const FVertexFactoryShaderPermutationParameters& Parameters);

private:
  virtual void InitRHI() override;
  virtual void ReleaseRHI() override;

  FCesiumPointAttenuationVertexBuffer VertexBuffer;
};
