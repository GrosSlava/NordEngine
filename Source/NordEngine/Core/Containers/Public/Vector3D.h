#pragma once

#include "EngineMath.h"
#include "NumericLimits.h"
#include "Axis.h"

#include "TArray.h"



struct FVector2D;
struct FPlane;
struct FRotator;
struct FQuat;

struct FVector4D;


/**
 * A vector in 3-D space composed of components (X, Y, Z) with floating point precision.
 */
struct FVector3D
{

public:

	//default constructor
	FORCEINLINE FVector3D() noexcept {}

	/**
	 * Constructor initializing all components to a single float value.
	 *
	 * @param InF Value to set all components to.
	 */
	FORCEINLINE FVector3D(float InF) :
		X(InF), Y(InF), Z(InF)
	{

	}

	//vector constructor
	FORCEINLINE FVector3D(float InX, float InY, float InZ) noexcept :
		X(InX), Y(InY), Z(InZ)
	{

	}

	//copy constructor
	FORCEINLINE FVector3D(const FVector3D& V) noexcept :
		X(V.X), Y(V.Y), Z(V.Z)
	{

	}

	/**
	 * Constructor using the XYZ components from a 4D vector.
	 *
	 * @param V 4D Vector to copy from.
	 */
	FVector3D(const FVector4D& V) noexcept;


//math operators
public:

	/**
	 * Gets the result of component-wise addition of this and another vector.
	 *
	 * @param V The vector to add to this.
	 * @return The result of vector addition.
	 */
	FORCEINLINE FVector3D operator+(const FVector3D& V) const noexcept
	{
		return FVector3D(X + V.X, Y + V.Y, Z + V.Z);
	}

	/**
	 * Gets the result of adding to each component of the vector.
	 *
	 * @param Bias How much to add to each component.
	 * @return The result of addition.
	*/
	FORCEINLINE FVector3D operator+(float Bias) const noexcept
	{
		return FVector3D(X + Bias, Y + Bias, Z + Bias);
	}

	/**
	 * Gets the result of component-wise subtraction of this by another vector.
	 *
	 * @param V The vector to subtract from this.
	 * @return The result of vector subtraction.
	 */
	FORCEINLINE FVector3D operator-(const FVector3D& V) const noexcept
	{
		return FVector3D(X - V.X, Y - V.Y, Z - V.Z);
	}

	/**
	 * Gets the result of subtracting from each component of the vector.
	 *
	 * @param Bias How much to subtract from each component.
	 * @return The result of subtraction.
	 */
	FORCEINLINE FVector3D operator-(float Bias) const noexcept
	{
		return FVector3D(X - Bias, Y - Bias, Z - Bias);
	}

	/**
	 * Get a negated copy of the vector.
	 *
	 * @return A negated copy of the vector.
	 */
	FORCEINLINE FVector3D operator-() const noexcept
	{
		return FVector3D(-X, -Y, -Z);
	}




	/**
	 * Adds another vector to this.
	 * Uses component-wise addition.
	 *
	 * @param V Vector to add to this.
	 * @return Copy of the vector after addition.
	 */
	FORCEINLINE FVector3D operator+=(const FVector3D& V)
	{
		X += V.X; Y += V.Y; Z += V.Z;
		return *this;
	}

	/**
	 * Subtracts another vector from this.
	 * Uses component-wise subtraction.
	 *
	 * @param V Vector to subtract from this.
	 * @return Copy of the vector after subtraction.
	 */
	FORCEINLINE FVector3D operator-=(const FVector3D& V)
	{
		X -= V.X; Y -= V.Y; Z -= V.Z;
		return *this;
	}

	/**
	 * Scales the vector.
	 *
	 * @param Scale Amount to scale this vector by.
	 * @return Copy of the vector after scaling.
	 */
	FORCEINLINE FVector3D operator*=(float Scale)
	{
		X *= Scale; Y *= Scale; Z *= Scale;
		return *this;
	}

	/**
	 * Divides the vector by a number.
	 *
	 * @param V What to divide this vector by.
	 * @return Copy of the vector after division.
	 */
	FORCEINLINE FVector3D operator/=(float V)
	{
		const float RV = 1.f / V;
		X *= RV; Y *= RV; Z *= RV;
		return *this;
	}

	/**
	 * Multiplies the vector with another vector, using component-wise multiplication.
	 *
	 * @param V What to multiply this vector with.
	 * @return Copy of the vector after multiplication.
	 */
	FORCEINLINE FVector3D operator*=(const FVector3D& V)
	{
		X *= V.X; Y *= V.Y; Z *= V.Z;
		return *this;
	}

	/**
	 * Divides the vector by another vector, using component-wise division.
	 *
	 * @param V What to divide vector by.
	 * @return Copy of the vector after division.
	 */
	FORCEINLINE FVector3D operator/=(const FVector3D& V)
	{
		X /= V.X; Y /= V.Y; Z /= V.Z;
		return *this;
	}





	/**
	 * Gets the result of scaling the vector (multiplying each component by a value).
	 *
	 * @param Scale What to multiply each component by.
	 * @return The result of multiplication.
	 */
	FORCEINLINE FVector3D operator*(float Scale) const  noexcept
	{
		return FVector3D(X * Scale, Y * Scale, Z * Scale);
	}

	/**
	 * Gets the result of dividing each component of the vector by a value.
	 *
	 * @param Scale What to divide each component by.
	 * @return The result of division.
	 */
	FORCEINLINE FVector3D operator/(float Scale) const noexcept
	{
		const float RScale = 1.f / Scale;
		return FVector3D(X * RScale, Y * RScale, Z * RScale);
	}

	/**
	 * Gets the result of component-wise multiplication of this vector by another.
	 *
	 * @param V The vector to multiply with.
	 * @return The result of multiplication.
	 */
	FORCEINLINE FVector3D operator*(const FVector3D& V) const noexcept
	{
		return FVector3D(X * V.X, Y * V.Y, Z * V.Z);
	}

	/**
	 * Gets the result of component-wise division of this vector by another.
	 *
	 * @param V The vector to divide by.
	 * @return The result of division.
	 */
	FORCEINLINE FVector3D operator/(const FVector3D& V) const noexcept
	{
		return FVector3D(X / V.X, Y / V.Y, Z / V.Z);
	}


	/**
	 * Calculate cross product between this and another vector.
	 *
	 * @param V The other vector.
	 * @return The cross product.
	 */
	FORCEINLINE FVector3D operator^(const FVector3D& V) const noexcept
	{
		return FVector3D
		(
			Y * V.Z - Z * V.Y,
			Z * V.X - X * V.Z,
			X * V.Y - Y * V.X
		);
	}

	/**
	 * Calculate the dot product between this and another vector.
	 *
	 * @param V The other vector.
	 * @return The dot product.
	 */
	FORCEINLINE float operator|(const FVector3D& V) const noexcept
	{
		return X * V.X + Y * V.Y + Z * V.Z;
	}


//check operators
public:

	/**
	 * Check against another vector for equality.
	 *
	 * @param V The vector to check against.
	 * @return true if the vectors are equal, false otherwise.
	 */
	FORCEINLINE bool operator==(const FVector3D& V) const
	{
		return X == V.X && Y == V.Y && Z == V.Z;
	}

	/**
	 * Check against another vector for inequality.
	 *
	 * @param V The vector to check against.
	 * @return true if the vectors are not equal, false otherwise.
	 */
	FORCEINLINE bool operator!=(const FVector3D& V) const
	{
		return X != V.X || Y != V.Y || Z != V.Z;
	}


	/**
	 * Checks whether all components of this vector are less than another.
	 *
	 * @param Other The vector to compare against.
	 * @return true if this is the smaller vector, otherwise false.
	 */
	FORCEINLINE bool operator<(const FVector3D& Other) const
	{
		return X < Other.X&& Y < Other.Y&& Z < Other.Z;
	}

	/**
	 * Checks whether all components of this vector are greater than another.
	 *
	 * @param Other The vector to compare against.
	 * @return true if this is the larger vector, otherwise false.
	 */
	FORCEINLINE bool operator>(const FVector3D& Other) const
	{
		return X > Other.X && Y > Other.Y && Z > Other.Z;
	}

	/**
	 * Checks whether all components of this vector are less than or equal to another.
	 *
	 * @param Other The vector to compare against.
	 * @return true if this vector is less than or equal to the other vector, otherwise false.
	 */
	FORCEINLINE bool operator<=(const FVector3D& Other) const
	{
		return X <= Other.X && Y <= Other.Y && Z <= Other.Z;
	}

	/**
	 * Checks whether all components of this vector are greater than or equal to another.
	 *
	 * @param Other The vector to compare against.
	 * @return true if this vector is greater than or equal to the other vector, otherwise false.
	 */
	FORCEINLINE bool operator>=(const FVector3D& Other) const
	{
		return X >= Other.X && Y >= Other.Y && Z >= Other.Z;
	}




public:

	/**
	 * Calculate the cross product of two vectors.
	 *
	 * @param A The first vector.
	 * @param B The second vector.
	 * @return The cross product.
	 */
	static FORCEINLINE FVector3D CrossProduct(const FVector3D& A, const FVector3D& B) noexcept
	{
		return A ^ B;
	}

	/**
	 * Calculate the dot product of two vectors.
	 *
	 * @param A The first vector.
	 * @param B The second vector.
	 * @return The dot product.
	 */
	static FORCEINLINE float DotProduct(const FVector3D& A, const FVector3D& B) noexcept
	{
		return A | B;
	}



	/**
	 * Compare two points and see if they're the same, using a threshold.
	 *
	 * @param P First vector.
	 * @param Q Second vector.
	 * @return Whether points are the same within a threshold. Uses fast distance approximation (linear per-component distance).
	 */
	static FORCEINLINE bool PointsAreSame(const FVector3D& P, const FVector3D& Q) noexcept
	{
		float Temp = P.X - Q.X;
		if ((Temp > -THRESH_POINTS_ARE_SAME) && (Temp < THRESH_POINTS_ARE_SAME))
		{
			Temp = P.Y - Q.Y;
			if ((Temp > -THRESH_POINTS_ARE_SAME) && (Temp < THRESH_POINTS_ARE_SAME))
			{
				Temp = P.Z - Q.Z;
				if ((Temp > -THRESH_POINTS_ARE_SAME) && (Temp < THRESH_POINTS_ARE_SAME))
				{
					return true;
				}
			}
		}
		return false;
	}

	/**
	 * Compare two points and see if they're within specified distance.
	 *
	 * @param Point1 First vector.
	 * @param Point2 Second vector.
	 * @param Dist Specified distance.
	 * @return Whether two points are within the specified distance. Uses fast distance approximation (linear per-component distance).
	 */
	static FORCEINLINE bool PointsAreNear(const FVector3D& Point1, const FVector3D& Point2, float Dist) noexcept
	{
		float Temp;
		Temp = (Point1.X - Point2.X); if (FMath::Abs(Temp) >= Dist) return false;
		Temp = (Point1.Y - Point2.Y); if (FMath::Abs(Temp) >= Dist) return false;
		Temp = (Point1.Z - Point2.Z); if (FMath::Abs(Temp) >= Dist) return false;
		return true;
	}

	/**
	 * Calculate the signed distance (in the direction of the normal) between a point and a plane.
	 *
	 * @param Point The Point we are checking.
	 * @param PlaneBase The Base Point in the plane.
	 * @param PlaneNormal The Normal of the plane (assumed to be unit length).
	 * @return Signed distance between point and plane.
	 */
	static FORCEINLINE float PointPlaneDist(const FVector3D& Point, const FVector3D& PlaneBase, const FVector3D& PlaneNormal) noexcept
	{
		return (Point - PlaneBase) | PlaneNormal;
	}

	/**
	 * Calculate the projection of a point on the given plane.
	 *
	 * @param Point The point to project onto the plane
	 * @param Plane The plane
	 * @return Projection of Point onto Plane
	 */
	static FVector3D PointPlaneProject(const FVector3D& Point, const FPlane& Plane) noexcept;
	

	/**
	 * Calculate the projection of a point on the plane defined by counter-clockwise (CCW) points A,B,C.
	 *
	 * @param Point The point to project onto the plane
	 * @param A 1st of three points in CCW order defining the plane
	 * @param B 2nd of three points in CCW order defining the plane
	 * @param C 3rd of three points in CCW order defining the plane
	 * @return Projection of Point onto plane ABC
	 */
	static FVector3D PointPlaneProject(const FVector3D& Point, const FVector3D& A, const FVector3D& B, const FVector3D& C) noexcept;


	/**
	* Calculate the projection of a point on the plane defined by PlaneBase and PlaneNormal.
	*
	* @param Point The point to project onto the plane
	* @param PlaneBase Point on the plane
	* @param PlaneNorm Normal of the plane (assumed to be unit length).
	* @return Projection of Point onto plane
	*/
	static FVector3D PointPlaneProject(const FVector3D& Point, const FVector3D& PlaneBase, const FVector3D& PlaneNormal) noexcept;
	

	/**
	 * Calculate the projection of a vector on the plane defined by PlaneNormal.
	 *
	 * @param  V The vector to project onto the plane.
	 * @param  PlaneNormal Normal of the plane (assumed to be unit length).
	 * @return Projection of V onto plane.
	 */
	static FORCEINLINE FVector3D VectorPlaneProject(const FVector3D& V, const FVector3D& PlaneNormal) noexcept
	{
		return V - V.ProjectOnToNormal(PlaneNormal);
	}



	/**
	 * Euclidean distance between two points.
	 *
	 * @param V1 The first point.
	 * @param V2 The second point.
	 * @return The distance between two points.
	 */
	static FORCEINLINE float Dist(const FVector3D& V1, const FVector3D& V2) noexcept
	{
		return FMath::Sqrt(FVector3D::DistSquared(V1, V2));
	}

	/**
	* Euclidean distance between two points in the XY plane (ignoring Z).
	*
	* @param V1 The first point.
	* @param V2 The second point.
	* @return The distance between two points in the XY plane.
	*/
	static FORCEINLINE float DistXY(const FVector3D& V1, const FVector3D& V2) noexcept
	{
		return FMath::Sqrt(FVector3D::DistSquaredXY(V1, V2));
	}
	static FORCEINLINE float Dist2D(const FVector3D& V1, const FVector3D& V2)  noexcept { return DistXY(V1, V2); }

	/**
	 * Squared distance between two points.
	 *
	 * @param V1 The first point.
	 * @param V2 The second point.
	 * @return The squared distance between two points.
	 */
	static FORCEINLINE float DistSquared(const FVector3D& V1, const FVector3D& V2) noexcept
	{
		return FMath::Square(V2.X - V1.X) + FMath::Square(V2.Y - V1.Y) + FMath::Square(V2.Z - V1.Z);
	}

	/**
	 * Squared distance between two points in the XY plane only.
	 *
	 * @param V1 The first point.
	 * @param V2 The second point.
	 * @return The squared distance between two points in the XY plane
	 */
	static FORCEINLINE float DistSquaredXY(const FVector3D& V1, const FVector3D& V2) noexcept
	{
		return FMath::Square(V2.X - V1.X) + FMath::Square(V2.Y - V1.Y);
	}
	static FORCEINLINE float DistSquared2D(const FVector3D& V1, const FVector3D& V2)  noexcept { return DistSquaredXY(V1, V2); }



	/**
	 * Compute pushout of a box from a plane.
	 *
	 * @param Normal The plane normal.
	 * @param Size The size of the box.
	 * @return Pushout required.
	 */
	static FORCEINLINE float BoxPushOut(const FVector3D& Normal, const FVector3D& Size) noexcept
	{
		return FMath::Abs(Normal.X * Size.X) + FMath::Abs(Normal.Y * Size.Y) + FMath::Abs(Normal.Z * Size.Z);
	}

	/**
	 * See if two normal vectors are nearly parallel, meaning the angle between them is close to 0 degrees.
	 *
	 * @param  Normal1 First normalized vector.
	 * @param  Normal1 Second normalized vector.
	 * @param  ParallelCosineThreshold Normals are parallel if absolute value of dot product (cosine of angle between them) is greater than or equal to this. For example: cos(1.0 degrees).
	 * @return true if vectors are nearly parallel, false otherwise.
	 */
	static FORCEINLINE bool Parallel(const FVector3D& Normal1, const FVector3D& Normal2, float ParallelCosineThreshold = THRESH_NORMALS_ARE_PARALLEL) noexcept
	{
		const float NormalDot = Normal1 | Normal2;
		return FMath::Abs(NormalDot) >= ParallelCosineThreshold;
	}

	/**
	 * See if two normal vectors are coincident (nearly parallel and point in the same direction).
	 *
	 * @param  Normal1 First normalized vector.
	 * @param  Normal2 Second normalized vector.
	 * @param  ParallelCosineThreshold Normals are coincident if dot product (cosine of angle between them) is greater than or equal to this. For example: cos(1.0 degrees).
	 * @return true if vectors are coincident (nearly parallel and point in the same direction), false otherwise.
	 */
	static FORCEINLINE bool Coincident(const FVector3D& Normal1, const FVector3D& Normal2, float ParallelCosineThreshold = THRESH_NORMALS_ARE_PARALLEL) noexcept
	{
		const float NormalDot = Normal1 | Normal2;
		return NormalDot >= ParallelCosineThreshold;
	}

	/**
	 * See if two normal vectors are nearly orthogonal (perpendicular), meaning the angle between them is close to 90 degrees.
	 *
	 * @param  Normal1 First normalized vector.
	 * @param  Normal2 Second normalized vector.
	 * @param  OrthogonalCosineThreshold Normals are orthogonal if absolute value of dot product (cosine of angle between them) is less than or equal to this. For example: cos(89.0 degrees).
	 * @return true if vectors are orthogonal (perpendicular), false otherwise.
	 */
	static FORCEINLINE bool Orthogonal(const FVector3D& Normal1, const FVector3D& Normal2, float OrthogonalCosineThreshold = THRESH_NORMALS_ARE_ORTHOGONAL) noexcept
	{
		const float NormalDot = Normal1 | Normal2;
		return FMath::Abs(NormalDot) <= OrthogonalCosineThreshold;
	}

	/**
	 * See if two planes are coplanar. They are coplanar if the normals are nearly parallel and the planes include the same set of points.
	 *
	 * @param Base1 The base point in the first plane.
	 * @param Normal1 The normal of the first plane.
	 * @param Base2 The base point in the second plane.
	 * @param Normal2 The normal of the second plane.
	 * @param ParallelCosineThreshold Normals are parallel if absolute value of dot product is greater than or equal to this.
	 * @return true if the planes are coplanar, false otherwise.
	 */
	static FORCEINLINE bool Coplanar(const FVector3D& Base1, const FVector3D& Normal1, const FVector3D& Base2, const FVector3D& Normal2, float ParallelCosineThreshold = THRESH_NORMALS_ARE_PARALLEL) noexcept
	{
		if (!FVector3D::Parallel(Normal1, Normal2, ParallelCosineThreshold)) return false;
		else if (FMath::Abs(FVector3D::PointPlaneDist(Base2, Base1, Normal1)) > THRESH_POINT_ON_PLANE) return false;
		else return true;
	}

	/**
	 * Triple product of three vectors: X dot (Y cross Z).
	 *
	 * @param X The first vector.
	 * @param Y The second vector.
	 * @param Z The third vector.
	 * @return The triple product: X dot (Y cross Z).
	 */
	static FORCEINLINE float Triple(const FVector3D& X, const FVector3D& Y, const FVector3D& Z) noexcept
	{
		return
		( (X.X * (Y.Y * Z.Z - Y.Z * Z.Y))
		+ (X.Y * (Y.Z * Z.X - Y.X * Z.Z))
		+ (X.Z * (Y.X * Z.Y - Y.Y * Z.X)));
	}


	/**
	 * Converts a vector containing radian values to a vector containing degree values.
	 *
	 * @param RadVector	Vector containing radian values
	 * @return Vector  containing degree values
	 */
	static FORCEINLINE FVector3D RadiansToDegrees(const FVector3D& RadVector) noexcept
	{
		return RadVector * (180.f / PI);
	}

	/**
	 * Converts a vector containing degree values to a vector containing radian values.
	 *
	 * @param DegVector	Vector containing degree values
	 * @return Vector containing radian values
	 */
	static FORCEINLINE FVector3D DegreesToRadians(const FVector3D& DegVector) noexcept
	{
		return DegVector * (PI / 180.f);
	}




	/**
	 * Generates a list of sample points on a Bezier curve defined by 2 points.
	 *
	 * @param ControlPoints	Array of 4 FVectors (vert1, controlpoint1, controlpoint2, vert2).
	 * @param NumPoints Number of samples.
	 * @param OutPoints Receives the output samples.
	 * @return The path length.
	 */
	static float EvaluateBezier(const FVector3D* ControlPoints, int32 NumPoints, TArray<FVector3D>& OutPoints) noexcept;
	
	/**
	 * Given a current set of cluster centers, a set of points, iterate N times to move clusters to be central.
	 *
	 * @param Clusters Reference to array of Clusters.
	 * @param Points Set of points.
	 * @param NumIterations Number of iterations.
	 * @param NumConnectionsToBeValid Sometimes you will have long strings that come off the mass of points
	 * which happen to have been chosen as Cluster starting points.  You want to be able to disregard those.
	 */
	static void GenerateClusterCenters(TArray<FVector3D>& Clusters, const TArray<FVector3D>& Points, int32 NumIterations, int32 NumConnectionsToBeValid) noexcept;

	/**
	 * Create an orthonormal basis from a basis with at least two orthogonal vectors.
	 * It may change the directions of the X and Y axes to make the basis orthogonal,
	 * but it won't change the direction of the Z axis.
	 * All axes will be normalized.
	 *
	 * @param XAxis The input basis' XAxis, and upon return the orthonormal basis' XAxis.
	 * @param YAxis The input basis' YAxis, and upon return the orthonormal basis' YAxis.
	 * @param ZAxis The input basis' ZAxis, and upon return the orthonormal basis' ZAxis.
	 */
	static void CreateOrthonormalBasis(FVector3D& XAxis, FVector3D& YAxis, FVector3D& ZAxis) noexcept;


public:

	/**
	 * Check against another vector for equality, within specified error limits.
	 *
	 * @param V The vector to check against.
	 * @param Tolerance Error tolerance.
	 * @return true if the vectors are equal within tolerance limits, false otherwise.
	 */
	FORCEINLINE bool Equals(const FVector3D& V, float Tolerance = KINDA_SMALL_NUMBER) const noexcept
	{
		return FMath::Abs(X - V.X) <= Tolerance && FMath::Abs(Y - V.Y) <= Tolerance && FMath::Abs(Z - V.Z) <= Tolerance;
	}

	/**
	 * Checks whether all components of this vector are the same, within a tolerance.
	 *
	 * @param Tolerance Error tolerance.
	 * @return true if the vectors are equal within tolerance limits, false otherwise.
	 */
	FORCEINLINE bool AllComponentsEqual(float Tolerance = KINDA_SMALL_NUMBER) const noexcept
	{
		return FMath::Abs(X - Y) <= Tolerance && FMath::Abs(X - Z) <= Tolerance && FMath::Abs(Y - Z) <= Tolerance;
	}

	
	
	/** Get a specific component of the vector, given a specific axis by enum */
	FORCEINLINE float GetComponentForAxis(EAxis Axis) const noexcept
	{
		switch (Axis)
		{

		case EAxis::X:	return X;
		case EAxis::Y:	return Y;
		case EAxis::Z:	return Z;
		default:		return 0.f;

		}
	}

	/** Set a specified componet of the vector, given a specific axis by enum */
	FORCEINLINE void SetComponentForAxis(EAxis Axis, float Component) noexcept
	{
		switch (Axis)
		{

		case EAxis::X:
			X = Component;
			break;
		case EAxis::Y:
			Y = Component;
			break;
		case EAxis::Z:
			Z = Component;
			break;

		}
	}


	/**
	 * Set the values of the vector directly.
	 *
	 * @param InX New X coordinate.
	 * @param InY New Y coordinate.
	 * @param InZ New Z coordinate.
	 */
	FORCEINLINE void Set(float InX, float InY, float InZ) noexcept
	{
		X = InX;
		Y = InY;
		Z = InZ;
	}




	/**
	 * Get the maximum value of the vector's components.
	 *
	 * @return The maximum value of the vector's components.
	 */
	FORCEINLINE float GetMax() const noexcept
	{
		return FMath::Max(FMath::Max(X, Y), Z);
	}

	/**
	 * Get the maximum absolute value of the vector's components.
	 *
	 * @return The maximum absolute value of the vector's components.
	 */
	FORCEINLINE float GetAbsMax() const noexcept
	{
		return FMath::Max(FMath::Max(FMath::Abs(X), FMath::Abs(Y)), FMath::Abs(Z));
	}

	/**
	 * Get the minimum value of the vector's components.
	 *
	 * @return The minimum value of the vector's components.
	 */
	FORCEINLINE float GetMin() const noexcept
	{
		return FMath::Min(FMath::Min(X, Y), Z);
	}

	/**
	 * Get the minimum absolute value of the vector's components.
	 *
	 * @return The minimum absolute value of the vector's components.
	 */
	FORCEINLINE float GetAbsMin() const noexcept
	{
		return FMath::Min(FMath::Min(FMath::Abs(X), FMath::Abs(Y)), FMath::Abs(Z));
	}

	/** Gets the component-wise min of two vectors. */
	FORCEINLINE FVector3D ComponentMin(const FVector3D& Other) const noexcept
	{
		return FVector3D(FMath::Min(X, Other.X), FMath::Min(Y, Other.Y), FMath::Min(Z, Other.Z));
	}

	/** Gets the component-wise max of two vectors. */
	FORCEINLINE FVector3D ComponentMax(const FVector3D& Other) const noexcept
	{
		return FVector3D(FMath::Max(X, Other.X), FMath::Max(Y, Other.Y), FMath::Max(Z, Other.Z));
	}

	/**
	 * Get a copy of this vector with absolute value of each component.
	 *
	 * @return A copy of this vector with absolute value of each component.
	 */
	FORCEINLINE FVector3D GetAbs() const noexcept
	{
		return FVector3D(FMath::Abs(X), FMath::Abs(Y), FMath::Abs(Z));
	}



	/**
	 * Get the length (magnitude) of this vector.
	 *
	 * @return The length of this vector.
	 */
	FORCEINLINE float Size() const noexcept
	{
		return FMath::Sqrt(X * X + Y * Y + Z * Z);
	}
	
	/**
	 * Get the squared length of this vector.
	 *
	 * @return The squared length of this vector.
	 */
	FORCEINLINE float SizeSquared() const noexcept
	{
		return X * X + Y * Y + Z * Z;
	}

	/**
	 * Get the length of the 2D components of this vector.
	 *
	 * @return The 2D length of this vector.
	 */
	FORCEINLINE float Size2D() const noexcept
	{
		return FMath::Sqrt(X * X + Y * Y);
	}

	/**
	 * Get the squared length of the 2D components of this vector.
	 *
	 * @return The squared 2D length of this vector.
	 */
	FORCEINLINE float SizeSquared2D() const noexcept
	{
		return X * X + Y * Y;
	}




	/**
	 * Checks whether vector is near to zero within a specified tolerance.
	 *
	 * @param Tolerance Error tolerance.
	 * @return true if the vector is near to zero, false otherwise.
	 */
	FORCEINLINE bool IsNearlyZero(float Tolerance = KINDA_SMALL_NUMBER) const noexcept
	{
		return
			FMath::Abs(X) <= Tolerance &&
			FMath::Abs(Y) <= Tolerance &&
			FMath::Abs(Z) <= Tolerance;
	}

	/**
	 * Checks whether all components of the vector are exactly zero.
	 *
	 * @return true if the vector is exactly zero, false otherwise.
	 */
	FORCEINLINE bool IsZero() const noexcept
	{
		return X == 0.f && Y == 0.f && Z == 0.f;
	}

	/**
	 * Check if the vector is of unit length, with specified tolerance.
	 *
	 * @param LengthSquaredTolerance Tolerance against squared length.
	 * @return true if the vector is a unit vector within the specified tolerance.
	 */
	FORCEINLINE bool IsUnit(float LengthSquaredTolerance = KINDA_SMALL_NUMBER) const noexcept
	{
		return FMath::Abs(1.0f - SizeSquared()) < LengthSquaredTolerance;
	}

	/**
	 * Checks whether vector is normalized.
	 *
	 * @return true if normalized, false otherwise.
	 */
	FORCEINLINE bool IsNormalized() const noexcept
	{
		return (FMath::Abs(1.f - SizeSquared()) < THRESH_VECTOR_NORMALIZED);
	}



	/**
	 * Calculates normalized version of vector without checking for zero length.
	 *
	 * @return Normalized version of vector.
	 * @see GetSafeNormal()
	 */
	FORCEINLINE FVector3D GetUnsafeNormal() const noexcept
	{
		const float Scale = FMath::InvSqrt(X * X + Y * Y + Z * Z);
		return FVector3D(X * Scale, Y * Scale, Z * Scale);
	}

	/**
	* Calculates normalized 2D version of vector without checking for zero length.
	*
	* @return Normalized version of vector.
	* @see GetSafeNormal2D()
	*/
	FORCEINLINE FVector3D GetUnsafeNormal2D() const noexcept
	{
		const float Scale = FMath::InvSqrt(X * X + Y * Y);
		return FVector3D(X * Scale, Y * Scale, 0);
	}

	/**
	 * Get a copy of the vector as sign only.
	 * Each component is set to +1 or -1, with the sign of zero treated as +1.
	 *
	 * @param A copy of the vector with each component set to +1 or -1
	 */
	FORCEINLINE FVector3D GetSignVector() const noexcept
	{
		return FVector3D
		(
			FMath::FloatSelect(X, 1.f, -1.f),
			FMath::FloatSelect(Y, 1.f, -1.f),
			FMath::FloatSelect(Z, 1.f, -1.f)
		);
	}

	/**
	 * Projects 2D components of vector based on Z.
	 *
	 * @return Projected version of vector based on Z.
	 */
	FORCEINLINE FVector3D Projection() const noexcept
	{
		const float RZ = 1.f / Z;
		return FVector3D(X * RZ, Y * RZ, 1);
	}


	/**
	 * Gets a copy of this vector snapped to a grid.
	 *
	 * @param GridSz Grid dimension.
	 * @return A copy of this vector snapped to a grid.
	 * @see FMath::GridSnap()
	 */
	FORCEINLINE FVector3D GridSnap(const float& GridSz) const noexcept
	{
		return FVector3D(FMath::GridSnap(X, GridSz), FMath::GridSnap(Y, GridSz), FMath::GridSnap(Z, GridSz));
	}

	/**
	 * Get a copy of this vector, clamped inside of a cube.
	 *
	 * @param Radius Half size of the cube.
	 * @return A copy of this vector, bound by cube.
	 */
	FORCEINLINE FVector3D BoundToCube(float Radius) const noexcept
	{
		return FVector3D
		(
			FMath::Clamp(X, -Radius, Radius),
			FMath::Clamp(Y, -Radius, Radius),
			FMath::Clamp(Z, -Radius, Radius)
		);

	}

	/** Get a copy of this vector, clamped inside of a cube. */
	FORCEINLINE FVector3D BoundToBox(const FVector3D& Min, const FVector3D Max) const noexcept
	{
		return FVector3D
		(
			FMath::Clamp(X, Min.X, Max.X),
			FMath::Clamp(Y, Min.Y, Max.Y),
			FMath::Clamp(Z, Min.Z, Max.Z)
		);
	}

	/**
	 * Add a vector to this and clamp the result in a cube.
	 *
	 * @param V Vector to add.
	 * @param Radius Half size of the cube.
	 */
	FORCEINLINE void AddBounded(const FVector3D& V, float Radius = MAX_int16) noexcept
	{
		*this = (*this + V).BoundToCube(Radius);
	}

	/**
	 * Check whether X, Y and Z are nearly equal.
	 *
	 * @param Tolerance Specified Tolerance.
	 * @return true if X == Y == Z within the specified tolerance.
	 */
	FORCEINLINE bool IsUniform(float Tolerance = KINDA_SMALL_NUMBER) const noexcept
	{
		return AllComponentsEqual(Tolerance);
	}



	/**
	 * Mirror a vector about a normal vector.
	 *
	 * @param MirrorNormal Normal vector to mirror about.
	 * @return Mirrored vector.
	 */
	FORCEINLINE FVector3D MirrorByVector(const FVector3D& MirrorNormal) const noexcept
	{
		return *this - MirrorNormal * (2.f * (*this | MirrorNormal));
	}

	/**
	 * Mirrors a vector about a plane.
	 *
	 * @param Plane Plane to mirror about.
	 * @return Mirrored vector.
	 */
	FVector3D MirrorByPlane(const FPlane& Plane) const noexcept;

	


	/**
	 * Returns the cosine of the angle between this vector and another projected onto the XY plane (no Z).
	 *
	 * @param B the other vector to find the 2D cosine of the angle with.
	 * @return The cosine.
	 */
	FORCEINLINE float CosineAngle2D(FVector3D B) const noexcept
	{
		FVector3D A(*this);
		A.Z = 0.0f;
		B.Z = 0.0f;
		A.Normalize();
		B.Normalize();
		return A | B;
	}

	/**
	 * Gets a copy of this vector projected onto the input vector.
	 *
	 * @param A	Vector to project onto, does not assume it is normalized.
	 * @return Projected vector.
	 */
	FORCEINLINE FVector3D ProjectOnTo(const FVector3D& A) const noexcept
	{
		return (A * ((*this | A) / (A | A)));
	}

	/**
	 * Gets a copy of this vector projected onto the input vector, which is assumed to be unit length.
	 *
	 * @param  Normal Vector to project onto (assumed to be unit length).
	 * @return Projected vector.
	 */
	FORCEINLINE FVector3D ProjectOnToNormal(const FVector3D& Normal) const noexcept
	{
		return (Normal * (*this | Normal));
	}


	/** When this vector contains Euler angles (degrees), ensure that angles are between +/-180 */
	FORCEINLINE void UnwindEuler() noexcept
	{
		X = FMath::UnwindDegrees(X);
		Y = FMath::UnwindDegrees(Y);
		Z = FMath::UnwindDegrees(Z);
	}

	/**
	 * Utility to check if there are any non-finite values (NaN or Inf) in this vector.
	 *
	 * @return true if there are any non-finite values in this vector, false otherwise.
	 */
	FORCEINLINE bool ContainsNaN() const noexcept
	{
		return (
			!FMath::IsFinite(X) ||
			!FMath::IsFinite(Y) ||
			!FMath::IsFinite(Z)
			);
	}







	/**
	 * Normalize this vector in-place if it is larger than a given tolerance. Leaves it unchanged if not.
	 *
	 * @param Tolerance Minimum squared length of vector for normalization.
	 * @return true if the vector was normalized correctly, false otherwise.
	 */
	bool Normalize(float Tolerance = SMALL_NUMBER) noexcept;

	/**
	 * Gets a normalized copy of the vector, checking it is safe to do so based on the length.
	 * Returns zero vector if vector length is too small to safely normalize.
	 *
	 * @param Tolerance Minimum squared vector length.
	 * @return A normalized copy if safe, (0,0,0) otherwise.
	 */
	FVector3D GetSafeNormal(float Tolerance = SMALL_NUMBER) const noexcept;

	/**
	 * Gets a normalized copy of the 2D components of the vector, checking it is safe to do so. Z is set to zero.
	 * Returns zero vector if vector length is too small to normalize.
	 *
	 * @param Tolerance Minimum squared vector length.
	 * @return Normalized copy if safe, otherwise returns zero vector.
	 */
	FVector3D GetSafeNormal2D(float Tolerance = SMALL_NUMBER) const noexcept;


	/**
	 * Util to convert this vector into a unit direction vector and its original length.
	 *
	 * @param OutDir Reference passed in to store unit direction vector.
	 * @param OutLength Reference passed in to store length of the vector.
	 */
	void ToDirectionAndLength(FVector3D& OutDir, float& OutLength) const noexcept;


	/** Create a copy of this vector, with its magnitude clamped between Min and Max. */
	FVector3D GetClampedToSize(float Min, float Max) const noexcept;


	/** Create a copy of this vector, with the 2D magnitude clamped between Min and Max. Z is unchanged. */
	FVector3D GetClampedToSize2D(float Min, float Max) const noexcept;


	/** Create a copy of this vector, with its maximum magnitude clamped to MaxSize. */
	FVector3D GetClampedToMaxSize(float MaxSize) const noexcept;


	/** Create a copy of this vector, with the maximum 2D magnitude clamped to MaxSize. Z is unchanged. */
	FVector3D GetClampedToMaxSize2D(float MaxSize) const noexcept;


	/**
	 * Gets the reciprocal of this vector, avoiding division by zero.
	 * Zero components are set to BIG_NUMBER.
	 *
	 * @return Reciprocal of this vector.
	 */
	FVector3D Reciprocal() const noexcept;


	/**
	 * Rotates around Axis (assumes Axis.Size() == 1).
	 *
	 * @param Angle Angle to rotate (in degrees).
	 * @param Axis Axis to rotate around.
	 * @return Rotated Vector.
	 */
	FVector3D RotateAngleAxis(const float AngleDeg, const FVector3D& Axis) const noexcept;
	
	/**
	 * Return the FRotator orientation corresponding to the direction in which the vector points.
	 * Sets Yaw and Pitch to the proper numbers, and sets Roll to zero because the roll can't be determined from a vector.
	 *
	 * @return FRotator from the Vector's direction, without any roll.
	 * @see ToOrientationQuat()
	 */
	FRotator ToOrientationRotator() const noexcept;

	/**
	 * Return the Quaternion orientation corresponding to the direction in which the vector points.
	 * Similar to the FRotator version, returns a result without roll such that it preserves the up vector.
	 *
	 * @note If you don't care about preserving the up vector and just want the most direct rotation, you can use the faster
	 * 'FQuat::FindBetweenVectors(FVector::ForwardVector, YourVector)' or 'FQuat::FindBetweenNormals(...)' if you know the vector is of unit length.
	 *
	 * @return Quaternion from the Vector's direction, without any roll.
	 * @see ToOrientationRotator(), FQuat::FindBetweenVectors()
	 */
	FQuat ToOrientationQuat() const noexcept;

	/**
	 * Return the FRotator orientation corresponding to the direction in which the vector points.
	 * Sets Yaw and Pitch to the proper numbers, and sets Roll to zero because the roll can't be determined from a vector.
	 * @note Identical to 'ToOrientationRotator()' and preserved for legacy reasons.
	 * @return FRotator from the Vector's direction.
	 * @see ToOrientationRotator(), ToOrientationQuat()
	 */
	FRotator Rotation() const noexcept;

	/**
	 * Converts a Cartesian unit vector into spherical coordinates on the unit sphere.
	 * @return Output Theta will be in the range [0, PI], and output Phi will be in the range [-PI, PI].
	 */
	FVector2D UnitCartesianToSpherical() const noexcept;
	

	/**
	 * Find good arbitrary axis vectors to represent U and V axes of a plane,
	 * using this vector as the normal of the plane.
	 *
	 * @param Axis1 Reference to first axis.
	 * @param Axis2 Reference to second axis.
	 */
	void FindBestAxisVectors(FVector3D& Axis1, FVector3D& Axis2) const noexcept;

	/**
	 * Convert a direction vector into a 'heading' angle.
	 *
	 * @return 'Heading' angle between +/-PI. 0 is pointing down +X.
	 */
	float HeadingAngle() const noexcept;


public:

	/** A zero vector (0,0,0) */
	static const FVector3D ZeroVector;

	/** One vector (1,1,1) */
	static const FVector3D OneVector;

	/**Uup vector (0,0,1) */
	static const FVector3D UpVector;

	/** Down vector (0,0,-1) */
	static const FVector3D DownVector;

	/** Forward vector (1,0,0) */
	static const FVector3D ForwardVector;

	/** Backward vector (-1,0,0) */
	static const FVector3D BackwardVector;

	/** Right vector (0,1,0) */
	static const FVector3D RightVector;

	/** Left vector (0,-1,0) */
	static const FVector3D LeftVector;

	/** Unit X axis vector (1,0,0) */
	static const FVector3D XAxisVector;

	/** Unit Y axis vector (0,1,0) */
	static const FVector3D YAxisVector;

	/** Unit Z axis vector (0,0,1) */
	static const FVector3D ZAxisVector;

public:

	/** Vector's X component. */
	float X = 0.0f;

	/** Vector's Y component. */
	float Y = 0.0f;

	/** Vector's Z component. */
	float Z = 0.0f;

};




/**
 * Multiplies a vector by a scaling factor.
 *
 * @param Scale Scaling factor.
 * @param V Vector to scale.
 * @return Result of multiplication.
 */
FORCEINLINE FVector3D operator*(float Scale, const FVector3D& V)
{
	return V.operator*(Scale);
}