#ifndef OBJECT
#define  OBJECT

class object
{
private:
	int kind;
	float posX, posY;
	float pre_posX, pre_posY;

	float sizeX, sizeY;

	float velX, velY;

	float VelMag;

	float accX, accY;
	float ForceX, ForceY;

	float frictionX;
	float frictionY;
	float mass;
	float coefFriction;
	float elapsed_time_in_sec;

	bool isVisible;

public:
	object();
	~object();
	void Update(float elapsed_time_in_sec);

	void SetLocation(float x, float y);
	void GetLocation(float *x, float *y);

	void SetPreLocation(float x, float y);
	void GetPreLocation(float *x, float *y);

	void SetSize(float size_x, float size_y);
	void GetSize(float *size_x, float *size_y);

	void SetVelocity(float velX, float velY);
	void GetVelocity(float *velX, float *velY);

	void SetAcc(float acc_x, float acc_y);
	void GetAcc(float *acc_x, float *acc_y);

	void SetForce(float ForceX, float ForceY);
	void GetForce(float *ForceX, float *ForceY);

	void SetMass(float Mass);
	void GetMass(float *Mass);

	void SetCoefFriction(float coefFriction);
	void GetCoefFriction(float *coefFriction);

	void SetKind(int kind);
	void GetKind(int *kind);

	void SetIsVisible(bool isVisible);
	bool GetIsVisible();

	void PingpongBall();

};

#endif // !OBJECT
