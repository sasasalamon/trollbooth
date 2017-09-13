Rails.application.routes.draw do
  root 'trollbooth#index'

  get 'trollbooth/index'
  post 'trollbooth/open'

  get :letmein, to: 'trollbooth#letmein'
end
