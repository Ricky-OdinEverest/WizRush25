// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectiles/TriangleExpander.h"

#include "ProceduralMeshComponent.h"


ATriangleExpander::ATriangleExpander()
{
    PrimaryActorTick.bCanEverTick = true;

    // Create mesh component and set as root
    ProcMesh = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("ProcMesh"));
    SetRootComponent(ProcMesh);

    // Grid parameters
    int32 R = 10;        // rows
    int32 C = 10;        // columns
    float Spacing = 100; // side length

    TArray<FVector> V; 
    TArray<int32> I; 
    TArray<FVector2D> UV;
    BuildTriangularGrid(R, C, Spacing, V, I, UV);

    TArray<FVector> Normals; Normals.Init(FVector(0,0,1), V.Num());
    TArray<FProcMeshTangent> Tangents; Tangents.Init(FProcMeshTangent(1,0,0), V.Num());
    TArray<FLinearColor> Colors; Colors.Init(FLinearColor::White, V.Num());

    ProcMesh->CreateMeshSection_LinearColor(
        0, V, I, Normals, UV, Colors, Tangents, true
    );
}

// Called when the game starts or when spawned
void ATriangleExpander::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATriangleExpander::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Make a triangular grid (equilateral tiling) lying in the XY plane (Z = 0)
// Rows = number of horizontal bands, Cols = vertices per row (before offset)
// a = side length (spacing)
void ATriangleExpander:: BuildTriangularGrid(
    int32 Rows, int32 Cols, float a,
    TArray<FVector>& Vertices, TArray<int32>& Indices,
    TArray<FVector2D>& UVs
){
    Vertices.Reset(); Indices.Reset(); UVs.Reset();
    Vertices.Reserve(Rows * Cols);
    UVs.Reserve(Rows * Cols);

    const float h = a * 0.866025403784f; // sqrt(3)/2

    auto Idx = [Cols](int32 r, int32 c){ return r * Cols + c; };

    // 1) Vertices (odd rows shifted by a/2)
    for (int32 r = 0; r < Rows; ++r) {
        const float xShift = (r & 1) ? (0.5f * a) : 0.0f;
        for (int32 c = 0; c < Cols; ++c) {
            const float x = xShift + c * a;
            const float y = r * h;
            Vertices.Add(FVector(x, y, 0.0f));
            UVs.Add(FVector2D(
                (x) / (a * (Cols - 1) + 0.5f * a),   // simple 0..1 mapping
                (y) / (h * (Rows - 1) + 1e-6f)
            ));
        }
    }

    // 2) Triangles
    // Between row r and r+1, we make “diamond” cells that split into two triangles.
    // The diagonal direction alternates by row parity to keep the mesh consistent.
    for (int32 r = 0; r < Rows - 1; ++r) {
        const bool even = ((r & 1) == 0);
        for (int32 c = 0; c < Cols - 1; ++c) {
            const int32 a0 = Idx(r,   c);
            const int32 a1 = Idx(r,   c+1);
            const int32 b0 = Idx(r+1, c);
            const int32 b1 = Idx(r+1, c+1);

            if (even) {
                // Diagonal goes from top-right (a1) to bottom-left (b0)
                // tri: a0, b0, a1
                Indices.Add(a0); Indices.Add(b0); Indices.Add(a1);
                // tri: b0, b1, a1
                Indices.Add(b0); Indices.Add(b1); Indices.Add(a1);
            } else {
                // Diagonal goes from top-left (a0) to bottom-right (b1)
                // tri: a0, b0, b1
                Indices.Add(a0); Indices.Add(b0); Indices.Add(b1);
                // tri: a0, b1, a1
                Indices.Add(a0); Indices.Add(b1); Indices.Add(a1);
            }
        }
    }
}
